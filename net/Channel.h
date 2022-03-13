// #include "EventLoop.h"
#include "CallBacks.h"

class EventLoop;
class Channel {
 public:
  Channel(int fd, EventLoop* loop);
  ~Channel();

  void setReadCallBack(EventCallBack func);
  void setWriteCallBack(EventCallBack func);
  void seterrorCallBack(EventCallBack func);
  void setRevent(int revent);
  void setPollindex(int idx);

  int getFd();
  int getEvent();
  int getPollindex();
  EventLoop* getLoop();

  void enableReading();
  void enableWriting();
  void disableWrting();
  void disableAll();

  bool Is_NoneEvent();
  bool Is_Reading();
  bool Is_Writing();

  void handleEvent();

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

  EventCallBack readCallBack;
  EventCallBack writeCallBack;
  EventCallBack errorCallBack;
};