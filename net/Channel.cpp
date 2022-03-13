#include "Channel.h"
#include "Poller.h"
#include "EventLoop.h"


const int Channel::NoneEvent = 0;
const int Channel::ReadEvent = POLLIN;
const int Channel::WriteEvent = POLLOUT;

Channel::Channel(int fd, EventLoop* loop)
    : fd_(fd),
      loop_(loop),
      event_(NoneEvent),
      revent_(NoneEvent),
      Pollindex(-1) {}

void Channel::setRevent(int revent) { revent_ = revent; }

void Channel::setPollindex(int idx) { Pollindex = idx; }

int Channel::getPollindex() { return Pollindex; }

int Channel::getFd() { return fd_; }

int Channel::getEvent() { return event_; }

bool Channel::Is_NoneEvent() { return !event_; }

void Channel::handleEvent() {
  if (revent_ & POLLIN) {
    if (readCallBack) readCallBack();
  }
  if (revent_ & POLLOUT) {
    if (writeCallBack) writeCallBack();
  }
}

void Channel::update() { loop_->updateChannel(this); }