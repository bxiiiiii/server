#include "HttpServer.h"

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

void defaultHttpCallback(const HttpRequest&, HttpResponse* resp){
  resp->setStatusCode(HttpResponse::k404NotFounnd);
  resp->setStatusMessage("Not Found");
  resp->setVersion("HTTP/1.1");
  resp->setConnection("close");
}

HttpServer::HttpServer(EventLoop* loop, const struct sockaddr_in& listenaddr)
    : server_(loop, listenaddr, "httpserver"),
      httpcallback_(defaultHttpCallback) {
  server_.setConnectionCallBack(
      std::bind(&HttpServer::onCoonnection, this, _1));
  server_.setMessageCallBack(
      std::bind(&HttpServer::onMessage, this, _1, _2, _3));
}

void HttpServer::start() { server_.start(); }

void HttpServer::onCoonnection(const TcpConnectionPtr& conn) {
  LOG_DEBUG << "SUCCESS";
}

void HttpServer::onMessage(const TcpConnectionPtr& conn, Buffer* buf,
                           Timestamp receiveTime) {
  std::string context(buf->retrieveAllAsString());
  LOG_DEBUG << context;
  praser.append(context.data(), context.size());
  HttpPraser::HTTP_CODE res = praser.prase();
  praser.getrequest().printrequest();
  if (res == HttpPraser::GET_REQUEST) {
    onRequest(conn, praser.getrequest());
  }
}

void HttpServer::onRequest(const TcpConnectionPtr& conn, const HttpRequest& rq){
  const std::string connection = rq.getHeader("Connection");
  HttpResponse response(conn);
  response.setConnection(connection);
  httpcallback_(rq, &response);
  Buffer buf;
  response.appendToBuffer(&buf);
  LOG_DEBUG << buf.peek();
  conn->send(buf.retrieveAllAsString());
  if(response.closeConnection()){
    conn->shutdown();
  }
}
