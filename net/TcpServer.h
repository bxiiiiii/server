#ifndef NET_TCPSERVER_H
#define NET_TCPSERVER_H

#include <map>

#include "TcpConnection.h"

class Acceptor;

class TcpServer {
 public:
  TcpServer(EventLoop* loop, const sockaddr_in& addr, const std::string& name);
  ~TcpServer();

  void start();

  void setConnectionCallBack(const ConnectionCallBack& callback);
  void setMessageCallBack(const MessageCallBack& callback);

  EventLoop* getloop();
  const std::string& getname();

 private:
  void newConnection(int sockfd, const struct sockaddr_in& peeraddr);
  void removeConnection(const TcpConnectionPtr& conn);
  void removeConnectionInLoop(const TcpConnectionPtr& conn);
  typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;
  EventLoop* loop_;
  const std::string name_;
  const struct sockaddr_in localaddr_;
  std::unique_ptr<Acceptor> acceptor_;
  ConnectionCallBack connectionCallBack_;
  MessageCallBack messageCallBack_;
  WriteCompleteCallBack writeCompleteCallBack_;
  ConnectionMap connections_;
  int nextConnId_;
};

#endif