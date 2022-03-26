#include <iostream>

#include "HttpServer.h"
#include "../net/Socket.h"

int main(){
  int port;
  std::cout << "port: ";
  std::cin >> port;
  EventLoop loop;
  struct sockaddr_in addr;
  socketopts::filladdr(&addr, port);
  // cout << addr.sin_port << " " << addr.sin_addr.s_addr << " " <<
  // addr.sin_family << endl;
  HttpServer server(&loop, addr);

  server.start();
  loop.loop();
  return 0;
}