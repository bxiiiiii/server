#include "Socket.h"

#include <errno.h>

#include <cstring>

int socketopts::createNoneblockingOrDie() {
  int sfd;
  sfd =
      ::socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK, IPPROTO_TCP);
  if (sfd < 0) {
    // TODO:log
  }

  return sfd;
}

void socketopts::bindOrDie(int sockfd, const struct sockaddr_in* addr) {
  if (::bind(sockfd, reinterpret_cast<const struct sockaddr*>(addr),
           sizeof(struct sockaddr_in)) < 0) {
    // TODO:log
  }
}

void socketopts::listenOrDie(int sockfd) {
  if (::listen(sockfd, MAX_LISTEN) < 0) {
    // TODO:log
  }
}

void socketopts::close(int sockfd)
{
  close(sockfd);
}

int socketopts::accept(int sockfd, const struct sockaddr_in* listenAddr) {
  int cfd;
  socklen_t clen;
  struct sockaddr_in caddr;

  cfd = ::accept4(sockfd, (struct sockaddr*)&caddr, &clen,
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

void socketopts::filladdr(struct sockaddr_in* addr) {
  memset(addr, 0, sizeof(struct sockaddr_in));
  addr->sin_family = AF_INET;
  addr->sin_port = htons(9999);
  addr->sin_addr.s_addr = htonl(INADDR_ANY);
}