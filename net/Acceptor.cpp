#include "Acceptor.h"

#include "Socket.h"
#include "EventLoop.h"
#include "../base/Logging.h"

Acceptor::Acceptor(EventLoop* loop, const struct sockaddr_in& listenAddr, bool reuseport)
    : loop_(loop),
      sockaddr_(listenAddr),
      sockfd_(socketopts::createNoneblockingOrDie()),
      acceptChannel(sockfd_, loop),
      listenning(false) {
  // TODO:set port reuse
  socketopts::setReusePort(sockfd_, reuseport);
  socketopts::bindOrDie(sockfd_, &sockaddr_);
  acceptChannel.setReadCallBack(std::bind(&Acceptor::handleRead, this));
}

Acceptor::~Acceptor() 
{
    acceptChannel.disableAll();
    acceptChannel.remove();
}

void Acceptor::setAcceptCallBack(const NewConnectionCallback& callback) {
  acceptcallback_ = std::move(callback);
}

void Acceptor::listen() {
  listenning = true;
  socketopts::listenOrDie(sockfd_);
  acceptChannel.enableReading();
}

bool Acceptor::listening() { return listenning; }

void Acceptor::handleRead() {
  int cfd;
  struct sockaddr_in caddr;
  cfd = socketopts::accept(sockfd_, &sockaddr_);
  LOG_DEBUG << "cfd : " << cfd;
  if (cfd > 0) {
    acceptcallback_(cfd, caddr);
  } else {
    socketopts::close(cfd);
  }
  // TODO:no fd
}