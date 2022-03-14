#include <arpa/inet.h>

#include "CallBacks.h"
#include "EventLoop.h"

class Acceptor {
 public:
  Acceptor(EventLoop* loop, const struct sockaddr_in& listenAddr);
  ~Acceptor();

  void setAcceptCallBack(AcceptCallback& callback);
  void listen();
  bool listening();

 private:
  void handleRead();

  int sockfd_;
  const struct sockaddr_in sockaddr_;
  EventLoop* loop_;
  Channel acceptChannel;
  bool listenning;
  AcceptCallback acceptcallback_;
};