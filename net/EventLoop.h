#include <iostream>
#include "Channel.h"
#include "CallBacks.h"

// class Channel;
class Poller;
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
    std::auto_ptr<Poller> poller_;
    ChannelList activechannels_;
};