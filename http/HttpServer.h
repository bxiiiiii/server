#ifndef HTTP_HTTPSERVER_H
#define HTTP_HTTPSERVER_H

#include "../net/TcpServer.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpPraser.h"

#include <functional>
#include <memory>

class HttpServer {
 public:
  typedef std::function<void(const HttpRequest&, HttpResponse*)> HttpCallback;
  HttpServer(EventLoop* loop, const struct sockaddr_in& listenaddr);
  void start();
  void setHttpCallback(const HttpCallback& cb);

 private:
  void onCoonnection(const TcpConnectionPtr& conn);
  void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp receiveTime);
  void onRequest(const TcpConnectionPtr& conn, const HttpRequest& rq);
  TcpServer server_;
  HttpCallback httpcallback_;
  HttpPraser praser;
};

#endif