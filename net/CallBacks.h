#include <functional>

typedef std::function<void(int socdfd)> ConnectionCallBack;
typedef std::function<void(int sockfd, const struct sockaddr_in&)>
    AcceptCallback;
typedef std::function<void()> EventCallBack;
typedef std::function<void()> MessageCallBack;
typedef std::function<void()> Functor;