#include "Acceptor.h"
#include "Socket.h"

Acceptor::Acceptor(EventLoop* loop, const struct sockaddr_in& listenAddr)
    : loop_(loop),
    sockaddr_(listenAddr),
    sockfd_(socketopts::createNoneblockingOrDie()),
    acceptChannel(sockfd_, loop),
    listenning(false)
{
    // socketopts::filladdr(&saddr);
    //TODO:set port reuse
    socketopts::bindOrDie(sockfd_, &sockaddr_);
    acceptChannel.setReadCallBack(std::bind(&Acceptor::handleRead, this));
}

void Acceptor::listen() {
  listenning = true;
  socketopts::listenOrDie(sockfd_);
}

void Acceptor::handleRead()
{
    int cfd;
    struct sockaddr_in caddr;
    cfd = socketopts::accept(sockfd_, &sockaddr_);
    if(cfd > 0){
        acceptcallback_(cfd, caddr);
    } else {
        socketopts::close(cfd);
    }
    //TODO:no fd
}