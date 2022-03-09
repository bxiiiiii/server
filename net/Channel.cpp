#include "Channel.h"
#include "Poller.h"

const int Channel::NoneEvent = 0;
const int Channel::ReadEvent = POLLIN;
const int Channel::WriteEvent = POLLOUT;

Channel::Channel(int fd, EventLoop* loop)
        : fd_(fd),
        loop_(loop),
        event_(NoneEvent),
        revent_(NoneEvent),
        index(-1)
{}

void Channel::handleEvent()
{
    if(revent_ & POLLIN){
        if(readCallBack) readCallBack();
    }
    if(revent_ & POLLOUT){
        if(writeCallBack) writeCallBack();
    }
}

void Channel::update()
{
    loop_->updateChannel(this);
}