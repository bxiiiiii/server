#include "HttpServer.h"

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

HttpServer::HttpServer(EventLoop* loop, const struct sockaddr_in& listenaddr)
    : server_(loop, listenaddr, "httpserver") {
  server_.setConnectionCallBack(
      std::bind(&HttpServer::onCoonnection, this, _1));
  server_.setMessageCallBack(
      std::bind(&HttpServer::onMessage, this, _1, _2, _3));
}

void HttpServer::start()
{
  server_.start();
}

void HttpServer::onCoonnection(const TcpConnectionPtr& conn)
{
LOG_DEBUG << "SUCCESS";
}

void HttpServer::onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp receiveTime){
   
  std::string context(buf->retrieveAllAsString());
   LOG_DEBUG << context;
   praser.append(context.data(), context.size());
    HttpPraser::HTTP_CODE res =  praser.prase();
   praser.getrequest().printrequest();
  if(res == )
}

