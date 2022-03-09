#include <poll.h>
#include <vector>

class Poller
{
public:
    typedef std::vector<Channel *>ChannelList;

    Poller(EventLoop *loop);
    ~Poller();
    int poll()
private:
    EventLoop* loop_;
    ChannelList activeChannels;
};