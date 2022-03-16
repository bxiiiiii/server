#include <functional>
#include <memory>

#include "Buffer.h"
#include "Timestamp.h"

typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallBack;
typedef std::function<void(int sockfd, const struct sockaddr_in&)>
    NewConnectionCallback;
typedef std::function<void()> EventCallBack;
typedef std::function<void(Timestamp)> ReadEventCallBack;
typedef std::function<void(const TcpConnectionPtr&, Buffer*, Timestamp)>
    MessageCallBack;
typedef std::function<void()> Functor;
typedef std::function<void(TcpConnectionPtr&)> CloseCallBack;

class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;