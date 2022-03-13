#include "Channel.h"

#include "EventLoop.h"
#include "Poller.h"

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(int fd, EventLoop* loop)
    : fd_(fd),
      loop_(loop),
      event_(kNoneEvent),
      revent_(kNoneEvent),
      Pollindex_(-1) {}

void Channel::setReadCallBack(EventCallBack func) { readCallBack = func; }

void Channel::setWriteCallBack(EventCallBack func) { writeCallBack = func; }

void Channel::seterrorCallBack(EventCallBack func) { errorCallBack = func; }

void Channel::setRevent(int revent) { revent_ = revent; }

void Channel::setPollindex(int idx) { Pollindex_ = idx; }

int Channel::getPollindex() { return Pollindex_; }

int Channel::getFd() { return fd_; }

int Channel::getEvent() { return event_; }

EventLoop* Channel::getLoop() { return loop_; }

void Channel::enableReading() {
  event_ |= kReadEvent;
  update();
}

void Channel::enableWriting() {
  event_ |= kWriteEvent;
  update();
}

void Channel::disableWrting() {
  event_ &= ~kNoneEvent;
  update();
}

void Channel::disableAll() {
  event_ = kNoneEvent;
  update();
}

bool Channel::Is_NoneEvent() { return !event_; }

bool Channel::Is_Writing() { return event_ & kWriteEvent; }

bool Channel::Is_Reading() { return event_ & kReadEvent; }

void Channel::handleEvent() {
  if (revent_ & POLLIN) {
    if (readCallBack) readCallBack();
  }
  if (revent_ & POLLOUT) {
    if (writeCallBack) writeCallBack();
  }
}

void Channel::update() { loop_->updateChannel(this); }