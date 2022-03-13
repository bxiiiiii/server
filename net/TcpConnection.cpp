#include "TcpConnection.h"
#include <unistd.h>


void TcpConnection::setMessageCallBack(MessageCallBack& callback)
{
    mescallback_ = callback;
}
    
void TcpConnection::setConnectionCallBack(ConnectionCallBack& callback)
{
    concallback_ = callback;
}  

void TcpConnection::handleRead()
{
    char buf[1024];
    int n = read(channel_->getFd(), buf, sizeof(buf));
    if(n > 0){
        mescallback_(shared_from_this(), buf, n);
    } else (n == 0){
        handleClose();
    } else {
        handleError();
    }
    
}

  void TcpConnection::handleWrite()
  {

  }
  void TcpConnection::handleClose()
  {
      channel_->disableAll();
      closecallback_(__cpp_lib_enable_shared_from_this());
  }
  void TcpConnection::handleError()
  {

  }