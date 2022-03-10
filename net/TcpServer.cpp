#include "TcpServer.h"

TcpServer::TcpServer(EventLoop* loop, const sockaddr_in& addr) : loop_(loop) {}

void start()
{

}
void TcpServer::setConnectionCallBack(const ConnectionCallBack& callback)
{
    connectionCallBack_ = callback; 
}

void TcpServer::setMessageCallBack(const MessageCallBack& callback)
{
    messageCallBack_ = callback; 
}

void TcpServer::newConnection(int sockfd, const struct sockaddr_in& addr)
{
    
}
