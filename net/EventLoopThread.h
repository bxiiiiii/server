#ifndef NET_EVENTLOOPTHREAD_H
#define NET_EVENTLOOPTHREAD_H
#include <condition_variable>
#include <mutex>

#include "../base/Thread.h"
#include "CallBacks.h"
class EventLoopThread {
 public:
  EventLoopThread(const ThreadInitCallback& callback = ThreadInitCallback(), const std::string& name = std::string());
  ~EventLoopThread();
  EventLoop* startLoop();

 private:
  void threadFunc();

  EventLoop* loop_;
  bool exiting_;
  Thread thread_;
  std::mutex mutex_;
  std::condition_variable cond_;
  ThreadInitCallback threadinitcallback_;
};

#endif