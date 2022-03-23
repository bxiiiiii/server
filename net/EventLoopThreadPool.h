
#ifndef NET_EVENTLOOPTHREADPOOL_H
#define NET_EVENTLOOPTHREADPOOL_H

#include "EventLoop.h"
#include "CallBacks.h"

class EventLoopThread;

class EventLoopThreadPool
{
    public:
    
    EventLoopThreadPool(EventLoop* baseloop,const std::string& name);
    ~EventLoopThreadPool();

    void setThreadNum(int numThreads);
    void start(const ThreadInitCallback& callback = ThreadInitCallback());

    EventLoop* getNextLoop();
    std::vector<EventLoop*> getAllloops();
    const std::string& getname() const;
    bool started() const;

private:
    EventLoop* baseloop_;
    std::string name_;
    bool started_;
    int numThreads_;
    int next_;
    std::vector<std::unique_ptr<EventLoopThread>> threads_;
    std::vector<EventLoop*> loops_;
};

#endif