#ifndef NET_EVENTLOOP_H
#define NET_EVENTLOOP_H
#include <iostream>
#include "CallBacks.h"
#include "../base/Timestamp.h"

// class Channel;
class Poller;
class Channel;
typedef std::vector<Channel*> ChannelList;

class EventLoop
{
public:
    EventLoop();
    ~EventLoop();
    void loop();
    void quit();

    void assertInLoopThread();
    bool isInLoopThread();

    void updateChannel(Channel *channel);
    void removeChannel(Channel* channel);

    void runInLoop(const Functor& callback);
    void queueInLoop(const Functor& callback);
    
private:
    void abortNotInLoopThread();
    // void handleRead();
    bool looping_;
    bool quit_;
    bool eventHandling_;
    const pid_t threadId;
    Timestamp ret_;
    std::unique_ptr<Poller> poller_;
    ChannelList activechannels_;
};

#endif