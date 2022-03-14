#include "TcpConnection.h"

#include <unistd.h>

#include <memory>

void TcpConnection::setMessageCallBack(MessageCallBack& callback) {
  mescallback_ = callback;
}

void TcpConnection::setConnectionCallBack(ConnectionCallBack& callback) {
  concallback_ = callback;
}

void TcpConnection::handleRead(Timestamp receiveTime) {
  char buf[1024];
  int Errno;
  // int n = read(channel_->getFd(), buf, sizeof(buf));
  int n = inputBuffer_.readFd(channel_->getFd(), &Errno);
  if (n > 0) {
    mescallback_(shared_from_this(), &inputBuffer_, receiveTime);
  } else
    (n == 0) { handleClose(); }
  else {
    handleError();
  }
}

void TcpConnection::handleWrite() {
  if (channel_->Is_Writing()) {
    ssize_t nwrote = write(channel_->getFd(), outputBuffer_.peek(),
                           outputBuffer_.readableBytes());
    if (nwrote > 0) {
      outputBuffer_.retrieve(nwrote);
      if (state_ == kDisconnecting) {
        shutdownInLoop();
      } else {
        // TODO:LOG
      }
    } else {
      // TODO:LOG
    }
  } else {
    // TODO:LOG
  }
}
void TcpConnection::handleClose() {
  channel_->disableAll();
  closecallback_(shared_from_this());
}
void TcpConnection::handleError() {}

void TcpConnection::send(const std::string& message) {
  if (state_ == kConnected) {
    if (loop_->isInLoopThread()) {
      sendInLoop(message);
    } else {
      loop_->runInLoop(std::bind(&TcpConnection::sendInLoop, this, message));
    }
  }
}

void TcpConnection::sendInLoop(const std::string& message) {
  ssize_t nwrote;
  if (!channel_->Is_Writing() && outputBuffer_.readableBytes() == 0) {
    nwrote = write(channel_->getFd(), message.data(), message.size());
    if (nwrote >= 0) {
      if (nwrote < message.size()) {
        // TODO:LOG
      } else {
        nwrote = 0;
        // TODO:log
      }
    }
  }

  if (nwrote < message.size()) {
    outputBuffer_.append(message.data() + nwrote, message.size() - nwrote);
    if (!channel_->Is_Writing()) {
      channel_->enableWriting();
    }
  }
}