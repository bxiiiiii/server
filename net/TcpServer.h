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
  void newConnection(int sockfd, const struct sockaddr_in& addr);
  typedef std::map<std::string, TcpConnection> ConnectionMap;
  EventLoop* loop_;
  ConnectionCallBack connectionCallBack_;
  MessageCallBack messageCallBack_;
  ConnectionMap connections;
};