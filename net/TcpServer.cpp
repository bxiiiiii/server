#include "TcpServer.h"

#include "Acceptor.h"
#include "EventLoop.h"

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

TcpServer::TcpServer(EventLoop* loop, const sockaddr_in& addr)
    : loop_(loop), acceptor_(new Acceptor(loop, addr)), localaddr_(addr) {
  acceptor_->setAcceptCallBack(
      std::bind(&TcpServer::newConnection, this, _1, _2));
}

TcpServer::~TcpServer() {
  for (auto& item : connections_) {
    TcpConnectionPtr conn(item.second);
    item.second.reset();
    conn->getLoop()->runInLoop(
        std::bind(&TcpConnection::connectDestroyed, conn));
  }
}

void TcpServer::start() {
  loop_->runInLoop(std::bind(&Acceptor::listen, acceptor_.get()));
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
  con->setCloseCallBack(std::bind(&TcpServer::removeConnection, this, _1));
  loop_->runInLoop(std::bind(&TcpConnection::connectEstablished, con));
}
void TcpServer::removeConnection(const TcpConnectionPtr& conn) {
  loop_->runInLoop(std::bind(&TcpServer::removeConnectionInLoop, this, conn));
}
void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn) {
  size_t n = connections_.erase(conn->getname());
  EventLoop* loop = conn->getLoop();
  loop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
}

EventLoop* TcpServer::getloop() { return loop_; }
const std::string& TcpServer::getname() { return name_; }