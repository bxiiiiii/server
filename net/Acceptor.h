#ifndef NET_ACCEPTOR_H
#define NET_ACCEPTOR_H
#include <arpa/inet.h>

#include "CallBacks.h"
#include "Channel.h"

class EventLoop;
class Acceptor {
 public:
  Acceptor(EventLoop* loop, const struct sockaddr_in& listenAddr);
  ~Acceptor();

  void setAcceptCallBack(const NewConnectionCallback& callback);
  void listen();
  bool listening();

 private:
  void handleRead();

  int sockfd_;
  const struct sockaddr_in sockaddr_;
  EventLoop* loop_;
  Channel acceptChannel;
  bool listenning;
   NewConnectionCallback acceptcallback_;
};

#endif