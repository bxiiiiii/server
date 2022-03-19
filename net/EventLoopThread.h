
#include "CallBacks.h"
class EventLoopThread
{
    public:
    EventLoopThread(const ThreadInitCallback & callback,char* name);

    EventLoop* startLoop();
};