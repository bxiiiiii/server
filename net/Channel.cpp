#include "Channel.h"

#include <unistd.h>

#include "../base/Logging.h"
#include "EventLoop.h"
#include "Poller.h"

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN;
const int Channel::kWriteEvent = EPOLLOUT;

Channel::Channel(int fd, EventLoop* loop)
    : loop_(loop),
      fd_(fd),
      event_(kNoneEvent),
      revent_(kNoneEvent),
      Pollindex_(-1),
      addedToLoop_(false),
      eventHanding_(false) {
  LOG_DEBUG << "";
}

Channel::~Channel() {
  LOG_DEBUG << "channel des";
  ::close(fd_);
};

void Channel::setReadCallBack(ReadEventCallBack func) { readCallBack = func; }

void Channel::setWriteCallBack(EventCallBack func) { writeCallBack = func; }

void Channel::seterrorCallBack(EventCallBack func) { errorCallBack = func; }

void Channel::setCloseCallBack(EventCallBack func) { closeCallBack = func; }

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

void Channel::disableReading() {
  event_ &= ~kReadEvent;
  update();
}

void Channel::disableWrting() {
  event_ &= ~kWriteEvent;
  update();
}

void Channel::disableAll() {
  event_ = kNoneEvent;
  update();
}

bool Channel::Is_NoneEvent() { return !event_; }

bool Channel::Is_Writing() { return event_ & kWriteEvent; }

bool Channel::Is_Reading() { return event_ & kReadEvent; }

void Channel::handleEvent(Timestamp receivetime) {
  eventHanding_ = true;
  if ((revent_ & EPOLLHUP) && !(revent_ & EPOLLIN)) {
    if (closeCallBack) closeCallBack();
  }
  if (revent_ & (EPOLLIN | EPOLLPRI | EPOLLHUP)) {
    if (readCallBack) {
      readCallBack(receivetime);
    }
    // readCallBack(receivetime);
  }
  if (revent_ & EPOLLOUT) {
    if (writeCallBack) writeCallBack();
  }
  eventHanding_ = false;
}

void Channel::update() {
  addedToLoop_ = true;
  loop_->updateChannel(this);
}

void Channel::remove() {
  addedToLoop_ = false;
  loop_->removeChannel(this);
}