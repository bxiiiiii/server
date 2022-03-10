#include <poll.h>

#include <map>
#include <vector>

#include "Channel.h"
#include "Timestamp.h"

class Poller {
 public:
  typedef std::vector<Channel*> ChannelList;

  Poller(EventLoop* loop);
  ~Poller();
  Timestamp poll(int time, ChannelList* activeChannel);
  void updateChannel(Channel* channel);

 private:
  void fileActiveChannels(int numEvents, ChannelList* activeChannels);

  EventLoop* loop_;
  std::vector<struct pollfd> PollFdList;
  std::map<int, Channel*> ChannelMap;
};