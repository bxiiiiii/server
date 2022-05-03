#include "Poller.h"

#include <syscall.h>
#include <unistd.h>

#include "../base/Logging.h"
#include "../base/Timestamp.h"
#include "Channel.h"

Poller::Poller(EventLoop* loop) : loop_(loop) {}

Poller::~Poller() = default;

Timestamp Poller::poll(int time, ChannelList* activeChannels) {
  int numEvents = ::poll(PollFdList.data(), PollFdList.size(), time);
  // LOG_DEBUG << "numevents : " << numEvents;
  Timestamp timestamp;
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

    LOG_DEBUG << "fd:" << channel->getFd() << " pi:" << channel->getPollindex()
              << " " << PollFdList.size() << " " << syscall(SYS_gettid);
  } else {
    int idx = channel->getPollindex();
    struct pollfd& event = PollFdList[idx];
    event.fd = channel->getFd();
    event.events = channel->getEvent();
    event.revents = 0;
    if (channel->Is_NoneEvent()) {
      event.fd = -1;
    }
  }
}

void Poller::removeChannel(Channel* channel) {
  int idx = channel->getPollindex();
  struct pollfd& event = PollFdList[idx];
  int n = ChannelMap.erase(channel->getFd());
  LOG_DEBUG << idx << " " << PollFdList.size();
  if (static_cast<size_t>(idx) == PollFdList.size() - 1) {
    PollFdList.pop_back();
  } else {
    int channelAtEnd = PollFdList.back().fd;
    iter_swap(PollFdList.begin() + idx, PollFdList.end() - 1);
    if (channelAtEnd < 0) {
      channelAtEnd = -channelAtEnd - 1;
    }
    ChannelMap[channelAtEnd]->setPollindex(idx);
    PollFdList.pop_back();
  }
}

void Poller::fileActiveChannels(int numEvents, ChannelList* activeChannel) {
  for (auto event : PollFdList) {
    if (numEvents == 0) break;
    if (event.revents > 0) {
      LOG_DEBUG << event.fd;
      auto ch = ChannelMap.find(event.fd);
      // for(auto i : ChannelMap){
      //   std::cout << i.first << " " << i.second->getFd() << std::endl;
      // }
      Channel* channel = ch->second;
      LOG_DEBUG << channel->getFd();
      channel->setRevent(event.revents);
      activeChannel->push_back(channel);
      --numEvents;
    }
  }
}