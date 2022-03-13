#include "CallBacks.h"
#include "EventLoop.h"
#include "Buffer.h"

#include <memory>
class TcpConnection {
 public:
  void setMessageCallBack(MessageCallBack& callback);
  void setConnectionCallBack(ConnectionCallBack& callback);
  void setCloseCallBack(CloseCallBack& callback);

 private:
  enum State { Connecting, Connected, Disconnected };
  void setState(State state) { state_ = state; }
  void handleRead();
  void handleWrite();
  void handleClose();
  void handleError();

  EventLoop* loop_;
  State state_;
  std::auto_ptr<Channel> channel_;
  struct sockaddr_in localaddr_;
  struct sockaddr_in peeraddr;
  MessageCallBack mescallback_;
  ConnectionCallBack concallback_;
  CloseCallBack closecallback_;
  Buffer inputBuffer_;
};