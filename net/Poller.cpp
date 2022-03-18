#include "Poller.h"
#include "Channel.h"

Poller::Poller(EventLoop* loop) : loop_(loop) {}

  Poller::~Poller() = default;

Timestamp Poller::poll(int time, ChannelList* activeChannels) {
  int numEvents = ::poll(PollFdList.data(), PollFdList.size() + 1, time);
  Timestamp timestamp(time);
  if (numEvents > 0) {
    fileActiveChannels(numEvents, activeChannels);
  }

  return timestamp;
}

void Poller::updateChannel(Channel* channel) {
  if (channel->getPollindex() < 0) {
    struct pollfd event;
    event.fd = channel->getFd();
    event.events = channel->getEvent();
    event.revents = 0;
    PollFdList.push_back(event);
    int idx = PollFdList.size() - 1;
    channel->setPollindex(idx);
    ChannelMap[event.fd] = channel;
  } else {
    int idx = channel->getPollindex();
    struct pollfd& event = PollFdList[idx];
    event.events = channel->getEvent();
    event.revents = 0;
    if (channel->Is_NoneEvent()) {
      event.fd = -1;
    }
  }
}

void Poller::removeChannel(Channel* channel) {
  int idx = channel->getFd();
  struct pollfd& event = PollFdList[idx];
  int n = ChannelMap.erase(idx);
  if (idx == PollFdList.size() - 1) {
    PollFdList.pop_back();
  } else {
    int channelAtEnd = PollFdList.back().fd;
    iter_swap(PollFdList.begin() + idx, PollFdList.end() - 1);
    if (channelAtEnd < 0) {
      channelAtEnd - -channelAtEnd - 1;
    }
    ChannelMap[channelAtEnd]->setPollindex(idx);
    PollFdList.pop_back();
  }
}

void Poller::fileActiveChannels(int numEvents, ChannelList* activeChannel) {
  for (auto event : PollFdList) {
    if (numEvents == 0) break;
    if (event.revents > 0) {
      auto ch = ChannelMap.find(event.fd);
      Channel* channel = ch->second;
      channel->setRevent(event.revents);
      activeChannel->push_back(channel);
      --numEvents;
    }
  }
}