#ifndef NET_CALLBACKS_H
#define NET_CALLBACKS_H
#include <functional>
#include <memory>

// #include "../base/Timestamp.h"
#include "Buffer.h"
class TcpConnection;
class EventLoop;
class Timestamp;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef std::function<void(int sockfd, const struct sockaddr_in&)>
    NewConnectionCallback;
typedef std::function<void()> EventCallBack;
typedef std::function<void(Timestamp)> ReadEventCallBack;
typedef std::function<void(const TcpConnectionPtr&)> CloseCallBack;
typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallBack;
typedef std::function<void(const TcpConnectionPtr&)> WriteCompleteCallBack;
typedef std::function<void(const TcpConnectionPtr&, Buffer*, Timestamp)>
    MessageCallBack;
typedef std::function<void()> Functor;

typedef std::function<void(EventLoop*)> ThreadInitCallback;

#endif