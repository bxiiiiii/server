#include "EventLoop.h"

#include <sys/syscall.h>
#include <unistd.h>

#include "Poller.h"

EventLoop::EventLoop() : looping(false), threadId(syscall(SYS_gettid)) {}

EventLoop::~EventLoop()
{}

void EventLoop::loop() {
  looping = true;
  quit_ = false;

  while(!quit_){
    activechannels_.clear();
    poller_->poll(0, &activechannels_);
    for(auto event : activechannels_){
      event->handleEvent();
    }
  }

  looping = false;
}

void EventLoop::assertInLoopThread()
{

}
bool EventLoop::isInLoopThread()
{

}

void EventLoop::updateChannel(Channel* channel) {
  poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel* channel)
{

}

void EventLoop::runInLoop(const Functor& callback) {
  if (isInLoopThread()) {
    callback();
  } else {
    queueInLoop(callback);
  }
}

void EventLoop::queueInLoop(const Functor& callback) 
{

}

void EventLoop::abortNotInLoopThread()
{

}