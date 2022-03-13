#include <arpa/inet.h>

#include <memory>

#include "Buffer.h"
#include "CallBacks.h"
#include "EventLoop.h"
class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
 public:
  void setMessageCallBack(MessageCallBack& callback);
  void setConnectionCallBack(ConnectionCallBack& callback);
  void setCloseCallBack(CloseCallBack& callback);

  void send(const std::string& message);
  void shutdown();

 private:
  enum State { kConnecting, kConnected, kDisconnecting, kDisconnected };
  void setState(State state) { state_ = state; }
  void handleRead(Timestamp receiveTime);
  void handleWrite();
  void handleClose();
  void handleError();

  void sendInLoop(const std::string& message);
  void shutdownInLoop();

  EventLoop* loop_;
  State state_;
  std::auto_ptr<Channel> channel_;
  struct sockaddr_in localaddr_;
  struct sockaddr_in peeraddr;
  MessageCallBack mescallback_;
  ConnectionCallBack concallback_;
  CloseCallBack closecallback_;
  Buffer inputBuffer_;
  Buffer outputBuffer_;
};