#ifndef EPOLL_H
#define EPOLL_H
#include <sys/epoll.h>

#include <map>
#include <vector>
class Channel;
class Timestamp;
class EventLoop;
typedef std::vector<Channel*> ChannelList;
typedef std::map<int, Channel*> ChannelMap;
typedef std::vector<struct epoll_event> EventList;

class Epoll {
 public:
  Epoll(EventLoop* loop);
  ~Epoll();
  Timestamp poll(int timeoutMs, ChannelList* activeChannels);
  void updateChannel(Channel* channel);
  void removeChannel(Channel* channel);

 private:
  void fillActiveChannels(int numEvents, ChannelList* activeChannels) const;
  void update(int operation, Channel* channel);

  EventLoop* loop_;
  ChannelMap channels_;
  int epollfd_;
  EventList events_;
};

#endif