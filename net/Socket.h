#include <arpa/inet.h>

#define MAX_LISTEN 1024
#define PORT 9999

namespace socketopts {
int createNoneblockingOrDie();
int bindOrDie(int sockfd, const struct sockaddr_in* addr);
int listenOrDie(int sockfd);
int close(int sockfd);
int accept(int sockfd, const struct sockaddr_in* listenAddr);
void filladdr(struct sockaddr_in* addr);
}