#include "Poller.h"

#include <Channel.h>
#include <unistd.h>

#include "../base/Timestamp.h"
#include "../base/Logging.h"

Epoll::Epoll(EventLoop* loop)
    : loop_(loop), epollfd_(::epoll_create1(EPOLL_CLOEXEC)), events_(16) {}

Epoll::~Epoll() { ::close(epollfd_); }

Timestamp Epoll::poll(int time, ChannelList* activeChannels) {
  int numEvents = ::epoll_wait(epollfd_, &*events_.begin(),
                               static_cast<int>(events_.size()), time);
  Timestamp now(Timestamp::now());
  if (numEvents > 0) {
    fillActiveChannels(numEvents, activeChannels);
    if (static_cast<size_t>(numEvents) == events_.size()) {
      events_.resize(events_.size() * 2);
    }
  }
  return now;
}

void Epoll::updateChannel(Channel* channel) {
  const int index = channel->getPollindex();
  int fd = channel->getFd();
  if (index == -1 || index == 2) {
    if (index == -1) {
      channels_[fd] = channel;
      LOG_DEBUG<<channels_.size();
    }
    channel->setPollindex(1);
    update(EPOLL_CTL_ADD, channel);
  } else {
    if (channel->Is_NoneEvent()) {
      update(EPOLL_CTL_DEL, channel);
      channel->setPollindex(2);
    } else {
      update(EPOLL_CTL_MOD, channel);
    }
  }
}
void Epoll::removeChannel(Channel* channel) {
  int fd = channel->getFd();
  int index = channel->getPollindex();
  size_t n = channels_.erase(fd);
  LOG_DEBUG << channels_.size();
  if (index == 1) {
    update(EPOLL_CTL_DEL, channel);
  }
  channel->setPollindex(-1);
}

void Epoll::update(int operation, Channel* channel) {
  struct epoll_event event;
  event.events = channel->getEvent();
  event.data.ptr = channel;
  int fd = channel->getFd();
  ::epoll_ctl(epollfd_, operation, fd, &event);
}

void Epoll::fillActiveChannels(int numEvents,
                               ChannelList* activeChannels) const {
  for (int i = 0; i < numEvents; ++i) {
    Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
    channel->setRevent(events_[i].events);
    activeChannels->push_back(channel);
  }
}