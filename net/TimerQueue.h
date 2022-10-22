#include <functional>
#include <queue>

#include "CallBacks.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Timer.h"
class TimerId;

struct TimerCmp {
  bool operator()(const std::shared_ptr<Timer>& a, std::shared_ptr<Timer>& b) {
    if (a->getExpiration() == b->getExpiration()) {
      return a->getSequence() > b->getSequence();
    } else {
      return a->getExpiration() > b->getExpiration();
    }
  }
};

class TimerQueue {
 public:
  using TimerList = std::priority_queue<std::shared_ptr<Timer>, std::vector<std::shared_ptr<Timer>>, TimerCmp>;
  TimerQueue(EventLoop* loop);
  ~TimerQueue();

  std::weak_ptr<Timer> addTimer(TimeCallBack cb, Timer::TimePoint when, Timer::TimeUnit interval);
  void cancel(std::weak_ptr<Timer>& timerId);

 private:
  void addTimerInLoop(const std::shared_ptr<Timer>& timer);
  void cancelTimerInLoop(const std::weak_ptr<Timer>& timer);

  void handleRead();
  std::vector<std::shared_ptr<Timer>> getExpired(const Timer::TimePoint& now);
  void reset(const std::vector<std::shared_ptr<Timer>> &expired, const Timer::TimePoint& now);
  bool insert(const std::shared_ptr<Timer>& timer);

  EventLoop* loop_;
  const int timerfd_;
  Channel timerfdChannel_;
  TimerList timers_;
};