#include "EventLoop.h"

#include <sys/eventfd.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <algorithm>

#include "../base/Logging.h"
#include "Channel.h"
#include "Poller.h"
#include "TimerQueue.h"

int createEventfd() {
  int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  if (evtfd < 0) {
    // LOG_SYSERR << "Failed in eventfd";
    abort();
  }
  return evtfd;
}

EventLoop::EventLoop()
    : looping_(false),
      threadId(syscall(SYS_gettid)),
      quit_(false),
      poller_(std::make_unique<Epoll>(this)),
      eventHandling_(false),
      callingPendingFunctors_(false),
      wakeupFd_(createEventfd()),
      wakeupChannel_(std::make_unique<Channel>(wakeupFd_, this)) {
  wakeupChannel_->setReadCallBack(std::bind(&EventLoop::handleRead, this));
  // we are always reading the wakeupfd
  wakeupChannel_->enableReading();
  LOG_DEBUG << threadId;
}

EventLoop::~EventLoop() {
  wakeupChannel_->disableAll();
  wakeupChannel_->remove();
  close(wakeupFd_);
}

void EventLoop::loop() {
  looping_ = true;
  quit_ = false;

  while (!quit_) {
    activechannels_.clear();
    ret_ = poller_->poll(1000, &activechannels_);
    eventHandling_ = true;
    for (auto event : activechannels_) {
      LOG_DEBUG << event->getFd();
      event->handleEvent(ret_);
    }
    eventHandling_ = false;
    doPendingFunctors();
  }

  looping_ = false;
}

void EventLoop::quit() {
  quit_ = true;
  if (!isInLoopThread()) {
    wakeup();
  }
}

void EventLoop::handleRead() {
  uint64_t one = 1;
  ssize_t n = ::read(wakeupFd_, &one, sizeof(one));
}

void EventLoop::assertInLoopThread() {}
bool EventLoop::isInLoopThread() {
  LOG_DEBUG << threadId << " " << syscall(SYS_gettid);
  return threadId == syscall(SYS_gettid);
}

void EventLoop::updateChannel(Channel* channel) {
  poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel* channel) {
  if (eventHandling_) {
    std::find(activechannels_.begin(), activechannels_.end(), channel);
  }
  poller_->removeChannel(channel);
}

void EventLoop::runInLoop(const Functor& callback) {
  if (isInLoopThread()) {
    callback();
  } else {
    queueInLoop(callback);
  }
}

void EventLoop::queueInLoop(const Functor& callback) {
  {
    std::lock_guard<std::mutex> lock(mutex_);
    pendingFunctors_.push_back(callback);
  }
  if (!isInLoopThread() || callingPendingFunctors_) {
    wakeup();
  }
}

void EventLoop::wakeup() {
  uint64_t one = 1;
  ssize_t n = write(wakeupFd_, &one, sizeof(one));
}

void EventLoop::abortNotInLoopThread() {}

void EventLoop::doPendingFunctors() {
  std::vector<Functor> functors;
  callingPendingFunctors_ = true;
  {
    std::lock_guard<std::mutex> lock(mutex_);
    functors.swap(pendingFunctors_);
  }
  for (const Functor& functor : functors) {
    functor();
  }
  callingPendingFunctors_ = false;
}

std::weak_ptr<Timer> EventLoop::runAt(
    const std::chrono::steady_clock::time_point& time_point, TimeCallBack cb) {
  return timerQueue_->addTimer(std::move(cb), time_point, Timer::TimeUnit());
}
std::weak_ptr<Timer> EventLoop::runAfter(const std::chrono::nanoseconds& delay,
                                         TimeCallBack cb) {
  Timer::TimePoint time = std::chrono::steady_clock::now() + delay;
  return timerQueue_->addTimer(std::move(cb), time, Timer::TimeUnit());
}
std::weak_ptr<Timer> EventLoop::runEvery(
    const std::chrono::nanoseconds& interval, TimeCallBack cb) {
  Timer::TimePoint time = std::chrono::steady_clock::now() + interval;
  return timerQueue_->addTimer(std::move(cb), time, interval);
}