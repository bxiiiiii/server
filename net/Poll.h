#ifndef NET_POLLER_H
#define NET_POLLER_H
#include <poll.h>

#include <map>
#include <vector>

// #include "Channel.h"
// #include "Timestamp.h"
class Channel;
class Timestamp;
class EventLoop;
typedef std::vector<Channel*> ChannelList;
class Poller {
 public:
  Poller(EventLoop* loop);
  ~Poller();
  Timestamp poll(int time, ChannelList* activeChannel);
  void updateChannel(Channel* channel);
  void removeChannel(Channel* channel);

 private:
  void fileActiveChannels(int numEvents, ChannelList* activeChannels);

  EventLoop* loop_;
  std::vector<struct pollfd> PollFdList;
  std::map<int, Channel*> ChannelMap;
};

#endif