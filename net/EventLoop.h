#include <iostream>

class EventLoop
{
public:
    EventLoop();
    ~EventLoop();
    void loop();
    void assertInLoopThread();
    bool isInLoopThread();
    void updateChannel(Channel *channel);
    
private:
    void abortNotInLoopThread();
    bool looping;
    const pid_t threadId;
};