#ifndef NET_EVENTLOOPTHREAD_H
#define NET_EVENTLOOPTHREAD_H
#include "CallBacks.h"
#include <mutex>
#include <condition_variable>
class EventLoopThread {
 public:
  EventLoopThread(const ThreadInitCallback& callback, char* name);

 private:
  void threadFunc();

  EventLoop* startLoop();
  std::mutex mutex_;
  std::condition_variable cond_;
  ThreadInitCallback threadinitcallback_;
};

#endif;