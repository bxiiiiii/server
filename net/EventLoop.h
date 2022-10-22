#ifndef NET_EVENTLOOP_H
#define NET_EVENTLOOP_H
#include <chrono>
#include <iostream>
#include <mutex>

#include "../base/Logging.h"
#include "../base/Timestamp.h"
#include "CallBacks.h"

// class Channel;
class Epoll;
class Channel;
class Timer;
class TimerQueue;
typedef std::vector<Channel*> ChannelList;

class EventLoop {
 public:
  EventLoop();
  ~EventLoop();
  void loop();
  void quit();

  void assertInLoopThread();
  bool isInLoopThread();
  bool eventHandling();

  void wakeup();
  void updateChannel(Channel* channel);
  void removeChannel(Channel* channel);

  void runInLoop(const Functor& callback);
  void queueInLoop(const Functor& callback);

  void printThreadID() { LOG_DEBUG << threadId; }

  // timer
  std::weak_ptr<Timer> runAt(
      const std::chrono::steady_clock::time_point& time_point, TimeCallBack cb);
  std::weak_ptr<Timer> runAfter(const std::chrono::nanoseconds& delay,
                                TimeCallBack cb);
  std::weak_ptr<Timer> runEvery(const std::chrono::nanoseconds& interval,
                                TimeCallBack cb);

 private:
  void abortNotInLoopThread();
  void doPendingFunctors();
  void handleRead();
  bool looping_;
  bool quit_;
  bool eventHandling_;
  bool callingPendingFunctors_;
  const pid_t threadId;
  Timestamp ret_;
  std::unique_ptr<Epoll> poller_;
  ChannelList activechannels_;

  int wakeupFd_;
  std::unique_ptr<Channel> wakeupChannel_;

  std::mutex mutex_;
  std::vector<Functor> pendingFunctors_;

  std::unique_ptr<TimerQueue> timerQueue_;
};

#endif