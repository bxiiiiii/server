#include "TcpConnection.h"

#include <unistd.h>

#include "../base/Logging.h"
#include "Channel.h"

TcpConnection::TcpConnection(EventLoop* loop, const std::string& name,
                             int sockfd, const sockaddr_in& localAddr,
                             const sockaddr_in& peerAddr)
    : loop_(loop),
      name_(name),
      state_(kConnecting),
      sockfd_(sockfd),
      channel_(new Channel(sockfd, loop)),
      localaddr_(localAddr),
      peeraddr_(peerAddr) {
  channel_->setReadCallBack(
      std::bind(&TcpConnection::handleRead, this, std::placeholders::_1));
  channel_->setWriteCallBack(std::bind(&TcpConnection::handleWrite, this));
  channel_->setCloseCallBack(std::bind(&TcpConnection::handleClose, this));
  channel_->seterrorCallBack(std::bind(&TcpConnection::handleError, this));
}

TcpConnection::~TcpConnection() = default;

EventLoop* TcpConnection::getLoop() { return loop_; }
const std::string& TcpConnection::getname() { return name_; }
const sockaddr_in& TcpConnection::getlocalAddr() { return localaddr_; }
const sockaddr_in& TcpConnection::getpeerAddr() { return peeraddr_; }
Buffer* TcpConnection::getinputbuffer() { return &inputBuffer_; }
Buffer* TcpConnection::getoutputbuffer() { return &outputBuffer_; }

bool TcpConnection::connected() { return state_ == kConnected; }
bool TcpConnection::disconnected() { return state_ == kDisconnected; }
void TcpConnection::connectEstablished() {
  LOG_DEBUG << "SUCCESS";
  setState(kConnected);
  channel_->enableReading();
  concallback_(shared_from_this());
}
void TcpConnection::connectDestroyed() {
  if (state_ == kConnected) {
    setState(kDisconnected);
    channel_->disableAll();
    concallback_(shared_from_this());
  }
  channel_->remove();
}

void TcpConnection::setMessageCallBack(const MessageCallBack& callback) {
  mescallback_ = callback;
}
void TcpConnection::setConnectionCallBack(const ConnectionCallBack& callback) {
  concallback_ = callback;
}
void TcpConnection::setCloseCallBack(const CloseCallBack& callback) {
  closecallback_ = callback;
}

void TcpConnection::setWriteCompleteCallBack(
    const WriteCompleteCallBack& callback) {
  writeCompletecallback_ = callback;
}

void TcpConnection::send(Buffer *buf){
  if(state_ == kConnected){
    if(loop_->isInLoopThread()){
      sendInLoop(buf->peek());
      buf->retrieveAll();
    }else{
      loop_->runInLoop(std::bind(&TcpConnection::sendInLoop, this, buf->retrieveAllAsString()));
    }
  }
}

void TcpConnection::send(const std::string& message) {
  if (state_ == kConnected) {
    if (loop_->isInLoopThread()) {
      sendInLoop(message);
    } else {
      loop_->runInLoop(std::bind(&TcpConnection::sendInLoop, this, message));
    }
  }
}

void TcpConnection::shutdown() {
  if (state_ == kConnected) {
    setState(kDisconnecting);
    loop_->runInLoop(std::bind(&TcpConnection::shutdownInLoop, this));
  }
}

void TcpConnection::startRead() {
  loop_->runInLoop(std::bind(&TcpConnection::startReadInLoop, this));
}
void TcpConnection::stopRead() {
  loop_->runInLoop(std::bind(&TcpConnection::stopReadInLoop, this));
}
bool TcpConnection::isReading() { return reading_; }

void TcpConnection::handleRead(Timestamp receiveTime) {
  int Errno;
  int n = inputBuffer_.readFd(channel_->getFd(), &Errno);
  if (n > 0) {
    LOG_DEBUG << n;
    mescallback_(shared_from_this(), &inputBuffer_, receiveTime);
  } else if (n == 0) {
    handleClose();
  } else {
    handleError();
  }
}
void TcpConnection::handleWrite() {
  if (channel_->Is_Writing()) {
    ssize_t nwrote = write(channel_->getFd(), outputBuffer_.peek(),
                           outputBuffer_.readableBytes());
    if (nwrote > 0) {
      outputBuffer_.retrieve(nwrote);
      if (outputBuffer_.readableBytes() == 0) {
        channel_->disableWrting();
        if (writeCompletecallback_) {
          loop_->queueInLoop(
              std::bind(writeCompletecallback_, shared_from_this()));
        }
        if (state_ == kDisconnecting) {
          shutdownInLoop();
        }
      }
    }else{
      //TODO:LOG
    }
  } else {
    // TODO:LOG
  }
}
void TcpConnection::handleClose() {
  setState(kDisconnected);
  channel_->disableAll();
  TcpConnectionPtr This(shared_from_this());
  closecallback_(This);
}
void TcpConnection::handleError() {
  //TODO:LOG
}

void TcpConnection::sendInLoop(const std::string& message) {
  // ssize_t nwrote;
  // if (!channel_->Is_Writing() && outputBuffer_.readableBytes() == 0) {
  //   nwrote = write(channel_->getFd(), message.data(), message.size());
  //   if (nwrote >= 0) {
  //     if (nwrote < message.size()) {
  //       // TODO:LOG
  //     } else {
  //       nwrote = 0;
  //       // TODO:log
  //     }
  //   }
  // }

  // if (nwrote < message.size()) {
  //   outputBuffer_.append(message.data() + nwrote, message.size() - nwrote);
  //   if (!channel_->Is_Writing()) {
  //     channel_->enableWriting();
  //   }
  // }
  size_t len = message.size();
  ssize_t nworte = 0;
  size_t remaining = message.size();
  if (state_ == kDisconnected) {
    LOG_WARN << "disconnected";
    return;
  }
  LOG_DEBUG << message;
  if (!channel_->Is_Writing() && outputBuffer_.readableBytes() == 0) {
    nworte = ::write(channel_->getFd(), message.data(), len);
    if (nworte >= 0) {
      remaining = len - nworte;
      if (remaining == 0 && writeCompletecallback_) {
        loop_->queueInLoop(
            std::bind(writeCompletecallback_, shared_from_this()));
      }
    } else {
      nworte = 0;
    }
  }
}
void TcpConnection::startReadInLoop() {
  if (!reading_ || !channel_->Is_Reading()) {
    channel_->enableReading();
    reading_ = true;
  }
}
void TcpConnection::stopReadInLoop() {
  if (reading_ || channel_->Is_Reading()) {
    channel_->disableReading();
    reading_ = false;
  }
}
void TcpConnection::shutdownInLoop() {
  if (!channel_->Is_Writing()) {
    ::shutdown(sockfd_, SHUT_WR);
  }
}