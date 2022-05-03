#ifndef HTTP_HTTPSERVER_H
#define HTTP_HTTPSERVER_H

#include <functional>
#include <iostream>
#include <memory>

#include "../base/AsyncLogging.h"
#include "../net/Socket.h"
#include "../net/TcpServer.h"
#include "HttpPraser.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

AsyncLogging* g_asyncLog = NULL;
void asyncOutput(const char* msg, int len) { g_asyncLog->append(msg, len); }

class HttpServer {
 public:
  typedef std::function<void(const HttpRequest&, HttpResponse*)> HttpCallback;
  HttpServer(EventLoop* loop, const struct sockaddr_in& listenaddr);
  void start();
  void setHttpCallback(const HttpCallback& cb);

 private:
  void onConnection(const TcpConnectionPtr& conn);
  void onMessage(const TcpConnectionPtr& conn, Buffer* buf,
                 Timestamp receiveTime);
  void onRequest(const TcpConnectionPtr& conn, const HttpRequest& rq);
  TcpServer server_;
  HttpCallback httpcallback_;
  HttpPraser praser;
};

int main() {
  AsyncLogging log("httpserver");
  log.start();
  g_asyncLog = &log;
  Logger::setOutput(asyncOutput);
  g_logLevel = Logger::INFO;

  int port;
  std::cout << "port: ";
  std::cin >> port;
  EventLoop loop;
  struct sockaddr_in addr;
  socketopts::filladdr(&addr, port);

  HttpServer server(&loop, addr);

  server.start();
  loop.loop();
  return 0;
}
#endif