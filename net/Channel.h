#ifndef NET_CHANNEL_H
#define NET_CHANNEL_H
#include "CallBacks.h"

class EventLoop;
class Channel {
 public:
  Channel(int fd, EventLoop* loop);
  ~Channel();

  void setReadCallBack(ReadEventCallBack func);
  void setWriteCallBack(EventCallBack func);
  void setCloseCallBack(EventCallBack func);
  void seterrorCallBack(EventCallBack func);
  void setRevent(int revent);
  void setPollindex(int idx);

  int getFd();
  int getEvent();
  int getPollindex();
  EventLoop* getLoop();

  void enableReading();
  void enableWriting();
  void disableReading();
  void disableWrting();
  void disableAll();

  bool Is_NoneEvent();
  bool Is_Reading();
  bool Is_Writing();

  void handleEvent(Timestamp receivetime);

  void remove();

 private:
  void update();

  static const int kNoneEvent;
  static const int kReadEvent;
  static const int kWriteEvent;

  EventLoop* loop_;
  int fd_;
  int event_;
  int revent_;
  int Pollindex_;

  bool addedToLoop_;
  bool eventHanding_;

  ReadEventCallBack readCallBack;
  EventCallBack writeCallBack;
  EventCallBack errorCallBack;
  EventCallBack closeCallBack;
};

#endif