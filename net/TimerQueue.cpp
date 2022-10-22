#include "TimerQueue.h"

#include <sys/timerfd.h>
#include <unistd.h>

#include "Logging.h"

int createTimerFd() {
  int timerFd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);

  if (timerFd < 0) {
    LOG_SYSFATAL << "timerfd_create failed.";
  }

  return timerFd;
}

void readTimerFd(int timerFd) {
  uint64_t buffer;
  ssize_t n = read(timerFd, &buffer, sizeof buffer);
  LOG_DEBUG << "TimerQueue::handleRead(): " << buffer;
  if (n != sizeof buffer) {
    LOG_ERROR << "TimerQueue::handleRead() reads " << n << "bytes instead of 8";
  }
}

void resetTimerFd(int timerFd, Timer::TimePoint expiration) {
  struct itimerspec newValue {};
  Timer::TimeUnit nsec = expiration - std::chrono::steady_clock::now();
  newValue.it_value.tv_sec =
      std::chrono::duration_cast<std::chrono::duration<int64_t>>(nsec).count();
  newValue.it_value.tv_nsec = nsec.count() % 1000000000;
  int ret = timerfd_settime(timerFd, 0, &newValue, nullptr);
  if (ret) {
    LOG_SYSERR << "timerfd_settime()";
  }
}

TimerQueue::TimerQueue(EventLoop* loop)
    : loop_(loop),
      timerfd_(createTimerFd()),
      timerfdChannel_(timerfd_, loop_),
      timers_() {
  timerfdChannel_.setReadCallBack(std::bind(&TimerQueue::handleRead, this));
}

TimerQueue::~TimerQueue() {
  timerfdChannel_.disableAll();
  timerfdChannel_.remove();
  close(timerfd_);
}

std::weak_ptr<Timer> TimerQueue::addTimer(TimeCallBack cb,
                                          Timer::TimePoint when,
                                          Timer::TimeUnit interval) {
  std::shared_ptr<Timer> timer =
      std::make_shared<Timer>(std::move(cb), when, interval);
  loop_->runInLoop([this, timer] { addTimerInLoop(timer); });
  return timer;
}

void TimerQueue::cancel(std::weak_ptr<Timer>& timerId) {
  loop_->runInLoop([this, timerId] { cancelTimerInLoop(timerId); });
}

void TimerQueue::handleRead() {
  loop_->assertInLoopThread();
  readTimerFd(timerfd_);
  std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
  std::vector<std::shared_ptr<Timer>> expired = getExpired(now);
  for (const auto& i : expired) {
    if (!i->getDelete()) i->run();
  }
  reset(expired, now);
}

void TimerQueue::addTimerInLoop(const std::shared_ptr<Timer>& timer) {
  loop_->assertInLoopThread();
  bool earliestChanged = insert(timer);

  if (earliestChanged) {
    resetTimerFd(timerfd_, timer->getExpiration());
  }
}

bool TimerQueue::insert(const std::shared_ptr<Timer>& timer) {
  loop_->assertInLoopThread();
  bool earliestChanged = false;
  if (!timers_.empty()) {
    if (timers_.top()->getExpiration() > timer->getExpiration()) {
      earliestChanged = true;
    }
    timers_.push(timer);
  } else {
    timers_.push(timer);
    earliestChanged = true;
  }
  return earliestChanged;
}

std::vector<std::shared_ptr<Timer>> TimerQueue::getExpired(
    const Timer::TimePoint& now) {
  std::vector<std::shared_ptr<Timer>> expired;
  while (!timers_.empty() && timers_.top()->getExpiration() <= now) {
    if (!timers_.top()->getDelete()) expired.emplace_back(timers_.top());
    timers_.pop();
  }
  return expired;
}

void TimerQueue::reset(const std::vector<std::shared_ptr<Timer>>& expired,
                       const Timer::TimePoint& now) {
  for (const auto& i : expired) {
    if (i->getRepeat() && !i->getDelete()) {
      i->restart(now);
      insert(i);
    }
  }
  while (!timers_.empty()) {
    if (!timers_.top()->getDelete()) {
      resetTimerFd(timerfd_, timers_.top()->getExpiration());
      break;
    } else {
      timers_.pop();
    }
  }
}

void TimerQueue::cancelTimerInLoop(const std::weak_ptr<Timer>& timerId) {
  if (!timerId.expired()) {
    auto guard = timerId.lock();
    guard->setDelete(true);
  }
}