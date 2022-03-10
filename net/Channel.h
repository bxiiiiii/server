// #include "EventLoop.h"
#include "CallBacks.h"

class EventLoop;
class Channel {
 public:
  Channel(int fd, EventLoop* loop);
  ~Channel();

  void setReadCallBack(EventCallBack func) { readCallBack = func; }
  void setWriteCallBack(EventCallBack func) { writeCallBack = func; }
  void seterrorCallBack(EventCallBack func) { errorCallBack = func; }

  void setRevent(int revent);
  void setPollindex(int idx);

  int getFd();
  int getEvent();
  int getPollindex();

  bool Is_NoneEvent();

  void handleEvent();

 private:
  void update();

  static const int NoneEvent;
  static const int ReadEvent;
  static const int WriteEvent;

  EventLoop* loop_;
  int fd_;
  int event_;
  int revent_;
  int Pollindex;

  EventCallBack readCallBack;
  EventCallBack writeCallBack;
  EventCallBack errorCallBack;
};