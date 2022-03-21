#include "EventLoop.h"

#include <sys/syscall.h>
#include <unistd.h>
#include <sys/eventfd.h>
#include <algorithm>

#include "Poller.h"
#include "Channel.h"
#include "../base/Logging.h"

int createEventfd()
{
  int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  if (evtfd < 0)
  {
    // LOG_SYSERR << "Failed in eventfd";
    abort();
  }
  return evtfd;
}

EventLoop::EventLoop() 
: looping_(false),
   threadId(syscall(SYS_gettid)),
   quit_(false),
   poller_(new Poller(this)),
   eventHandling_(false),
   callingPendingFunctors_(false),
   wakeupFd_(createEventfd()),
   wakeupChannel_(new Channel(wakeupFd_, this)) {
  wakeupChannel_->setReadCallBack(
      std::bind(&EventLoop::handleRead, this));
  // we are always reading the wakeupfd
  wakeupChannel_->enableReading();
   }

EventLoop::~EventLoop() {}

void EventLoop::loop() {
  looping_ = true;
  quit_ = false;

  while (!quit_) {
    activechannels_.clear();
    ret_ = poller_->poll(-1, &activechannels_);
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

void EventLoop::quit() { quit_ = true; }

void EventLoop::handleRead()
{
  uint64_t one = 1;
  ssize_t n = ::read(wakeupFd_, &one, sizeof one);
}

void EventLoop::assertInLoopThread() {}
bool EventLoop::isInLoopThread() 
{
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
    pendingFunctors_.push_back(callback);}
  if(!isInLoopThread() || callingPendingFunctors_){
    wakeup();
  }
}

void EventLoop::wakeup()
{
  uint64_t one = 1;
  ssize_t n = write(wakeupFd_, &one, sizeof one);

}

void EventLoop::abortNotInLoopThread() {}

void EventLoop::doPendingFunctors()
{
  std::vector<Functor> functors;
  callingPendingFunctors_ = true;
  {
std::lock_guard<std::mutex> lock(mutex_);
    functors.swap(pendingFunctors_);
  }
  for(const Functor& functor : functors){
    functor();
  }
  callingPendingFunctors_ = false;
}