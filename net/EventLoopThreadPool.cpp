#include "EventLoopThreadPool.h"

#include "EventLoopThread.h"

EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseloop)
    : baseloop_(baseloop), started_(false), numThreads_(0), next_(0) {}

EventLoopThreadPool::~EventLoopThreadPool()
{

}

void EventLoopThreadPool::start(const ThreadInitCallback& callback)
{
    started_ = true;
    for(int i = 0; i < numThreads_; i++){
        char buf[name_.size() + 32];
        snprintf(buf, sizeof buf, "%s%d", name_.c_str(), i);
        EventLoopThread* t = new EventLoopThread(callback, buf);
        threads_.push_back(std::unique_ptr<EventLoopThread>(t));
        loops_.push_back(t->startLoop());
    }
    if(numThreads_ == 0 && callback){
        callback(baseloop_);
    }
}

EventLoop* EventLoopThreadPool::getNextLoop()
{
    EventLoop* loop = baseloop_;
    if(!loops_.empty()){
        loop = loops_[next_];
        ++next_;
        if((next_) >= loops_.size()){
            next_ = 0;
        }
    }

    return loop;
}

std::vector<EventLoop*> EventLoopThreadPool::getAllloops()
{
    if(loops_.empty()){
        return std::vector<EventLoop*>(1, baseloop_);
    } else {
        return loops_;
    }
}

void EventLoopThreadPool::setThreadNum(int numThreads)
{
    numThreads_ = numThreads;
}

const std::string& EventLoopThreadPool::getname() const
{
    return name_;
}

bool EventLoopThreadPool::started() const
{
    return started_;
}