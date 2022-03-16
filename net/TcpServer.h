#include <map>

#include "Acceptor.h"
#include "CallBacks.h"
#include "EventLoop.h"
#include "TcpConnection.h"

class TcpServer {
 public:
  TcpServer(EventLoop* loop, const sockaddr_in& addr);
  ~TcpServer();

  void start();

  void setConnectionCallBack(const ConnectionCallBack& callback);
  void setMessageCallBack(const MessageCallBack& callback);

 private:
  void newConnection(int sockfd, const struct sockaddr_in& peeraddr);
  typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;
  EventLoop* loop_;
  const struct  sockaddr_in localaddr_;
  std::auto_ptr<Acceptor> acceptor_;
  ConnectionCallBack connectionCallBack_;
  MessageCallBack messageCallBack_;
  ConnectionMap connections_;
};