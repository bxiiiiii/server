#include <EventLoopThread.h>

#include "EventLoop.h"

EventLoopThread::EventLoopThread(const ThreadInitCallback& cb, const std::string& name)
    : loop_(nullptr),
      exiting_(false),
      thread_(std::bind(&EventLoopThread::threadFunc, this)),
      threadinitcallback_(cb) {}

EventLoopThread::~EventLoopThread() {
  exiting_ = true;
  if (loop_) {
    loop_->quit();
    thread_.join();
  }
}
EventLoop* EventLoopThread::startLoop() {
  thread_.start();
  EventLoop* loop = nullptr;
  {
    std::unique_lock<std::mutex> lock(mutex_);
    while (loop_ == nullptr) {
      cond_.wait(lock);
    }
    loop_ = loop;
  }
  return loop;
}

void EventLoopThread::threadFunc() {
  EventLoop loop;
  if (threadinitcallback_) {
    threadinitcallback_(&loop);
  }

  {
    std::unique_lock<std::mutex> lock(mutex_);
    loop_ = &loop;
    cond_.notify_one();
  }

  loop.loop();
  std::unique_lock<std::mutex> lock(mutex_);
  loop_ = nullptr;
}