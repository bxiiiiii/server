#include "TcpServer.h"

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

TcpServer::TcpServer(EventLoop* loop, const sockaddr_in& addr)
    : loop_(loop), acceptor_(new Acceptor(loop, addr)) {
  acceptor_->setAcceptCallBack(
      std::bind(&TcpServer::newConnection, this, _1, _2));
}

void TcpServer::start() {
  loop_->runInLoop(std::bind(&Acceptor::listen, acceptor_));
}
void TcpServer::setConnectionCallBack(const ConnectionCallBack& callback) {
  connectionCallBack_ = callback;
}

void TcpServer::setMessageCallBack(const MessageCallBack& callback) {
  messageCallBack_ = callback;
}

void TcpServer::newConnection(int sockfd, const struct sockaddr_in& peeraddr) {
  std::string conName = "gg";

  TcpConnectionPtr con(
      new TcpConnection(loop_, conName, sockfd, localaddr_, peeraddr));
  connections_[conName] = con;
  con->setConnectionCallBack(connectionCallBack_);
  con->setMessageCallBack(messageCallBack_);
}
