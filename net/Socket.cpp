#include "Socket.h"

#include <errno.h>

#include <cstring>

using namespace socketopts;

int createNoneblockingOrDie() {
  int sfd;
  sfd =
      socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK, IPPROTO_TCP);
  if (sfd < 0) {
    // TODO:log
  }

  return sfd;
}

int bindOrDie(int sockfd, const struct sockaddr_in* addr) {
  if (bind(sockfd, reinterpret_cast<const struct sockaddr*>(addr),
           sizeof(struct sockaddr_in)) < 0) {
    // TODO:log
  }
}

int listenOrDie(int sockfd) {
  if (listen(sockfd, MAX_LISTEN) < 0) {
    // TODO:log
  }
}

int close(int sockfd);

int accept(int sockfd, struct sockaddr_in* listenAddr) {
  int cfd;
  socklen_t clen;
  struct sockaddr_in caddr;

  cfd = accept4(sockfd, (struct sockaddr*)&caddr, &clen,
                SOCK_CLOEXEC | SOCK_NONBLOCK);

  if (cfd < 0) {
    int errornum = errno;
    switch (errornum) {
      case EBADF:
        /* code */
        break;
      case EFAULT:

      // TODO
      default:
        break;
    }
  }

  return cfd;
}

void filladdr(struct sockaddr_in* addr) {
  memset(addr, 0, sizeof(struct sockaddr_in));
  addr->sin_family = AF_INET;
  addr->sin_port = htons(9999);
  addr->sin_addr.s_addr = htonl(INADDR_ANY);
}