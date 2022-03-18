#include "EventLoop.h"

#include <sys/syscall.h>
#include <unistd.h>

#include <algorithm>

#include "Poller.h"
#include "Channel.h"

EventLoop::EventLoop() 
: looping_(false),
   threadId(syscall(SYS_gettid)),
   quit_(false),
   poller_(new Poller(this)),
   eventHandling_(false) {}

EventLoop::~EventLoop() {}

void EventLoop::loop() {
  looping_ = true;
  quit_ = false;

  while (!quit_) {
    activechannels_.clear();
    ret_ = poller_->poll(0, &activechannels_);
    for (auto event : activechannels_) {
      event->handleEvent(ret_);
    }
  }

  looping_ = false;
}

void EventLoop::quit() { quit_ = true; }

void EventLoop::assertInLoopThread() {}
bool EventLoop::isInLoopThread() 
{
  return true;
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

void EventLoop::queueInLoop(const Functor& callback) {}

void EventLoop::abortNotInLoopThread() {}