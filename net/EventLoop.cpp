#include "EventLoop.h"

EventLoop::EventLoop() 
    : looping(false)
{
}

~EventLoop();
void loop();
void assertInLoopThread();
bool isInLoopThread();
void EventLoop::updateChannel(Channel *channel)
{
    // channel->update();
}
void EventLoop::runInLoop(const Functor& callback){
    if(isInLoopThread()){
        callback();
    } else{
        queueInLoop(callback);
    }
}

void EventLoop::queueInLoop(const Functor& callback)
{

}