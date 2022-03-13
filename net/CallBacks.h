#include <functional>
#include <memory>

#include "Buffer.h"
#include "Timestamp.h"

typedef std::function<void(int socdfd)> ConnectionCallBack;
typedef std::function<void(int sockfd, const struct sockaddr_in&)>
    AcceptCallback;
typedef std::function<void()> EventCallBack;
typedef std::function<void(const TcpConnectionPtr&, Buffer* buf, Timestamp)>
    MessageCallBack;
typedef std::function<void()> Functor;
typedef std::function<void()> CloseCallBack;

class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;