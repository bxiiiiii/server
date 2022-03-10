#include <iostream>
#include "Channel.h"
#include "CallBacks.h"

// class Channel;
class EventLoop
{
public:
    EventLoop();
    ~EventLoop();
    void loop();
    void assertInLoopThread();
    bool isInLoopThread();
    void updateChannel(Channel *channel);
    void runInLoop(const Functor& callback);
    void queueInLoop(const Functor& callback);
    
private:
    void abortNotInLoopThread();
    bool looping;
    const pid_t threadId;
};