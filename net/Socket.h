#ifndef NET_SOCKET_H
#define NET_SOCKET_H
#include <arpa/inet.h>

#define MAX_LISTEN 1024
#define PORT 9999

namespace socketopts {
int createNoneblockingOrDie();
void bindOrDie(int sockfd, const struct sockaddr_in* addr);
void listenOrDie(int sockfd);
void close(int sockfd);
int accept(int sockfd, const struct sockaddr_in* listenAddr);
void filladdr(struct sockaddr_in* addr, int port);
void setReusePort(int sockfd, bool on);
}

#endif