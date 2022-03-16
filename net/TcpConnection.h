#include <arpa/inet.h>

#include <memory>

#include "Buffer.h"
#include "CallBacks.h"
#include "EventLoop.h"
class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
 public:
  TcpConnection(EventLoop* loop, const std::string& name, int sockfd,
                const sockaddr_in& localAddr, const sockaddr_in& peerArrd);
  ~TcpConnection();

  EventLoop* getLoop();
  const std::string& getname();
  const sockaddr_in getlocalAddr();
  const sockaddr_in getpeerAddr();
  Buffer* getinputbuffer();
  Buffer* getoutputbuffer();

  bool connected();
  bool disconnected();
  void connectEstavlished();
  void connectDestroyed();

  void setMessageCallBack(MessageCallBack& callback);
  void setConnectionCallBack(ConnectionCallBack& callback);
  void setCloseCallBack(CloseCallBack& callback);

  void send(const std::string& message);
  void send(Buffer* message);
  void shutdown();

  void startRead();
  void stopRead();
  bool isReading();

  void connectEstablished();
  void connectDestroyed();

 private:
  enum State { kConnecting, kConnected, kDisconnecting, kDisconnected };
  void setState(State state) { state_ = state; }
  void handleRead(Timestamp receiveTime);
  void handleWrite();
  void handleClose();
  void handleError();

  void sendInLoop(const std::string& message);
  void startReadInLoop();
  void stopReadInLoop();
  void shutdownInLoop();

  EventLoop* loop_;
  State state_;
  bool reading_;
  const std::string name_;
  int sockfd_;
  std::auto_ptr<Channel> channel_;
  struct sockaddr_in localaddr_;
  struct sockaddr_in peeraddr_;
  MessageCallBack mescallback_;
  ConnectionCallBack concallback_;
  CloseCallBack closecallback_;
  Buffer inputBuffer_;
  Buffer outputBuffer_;
};