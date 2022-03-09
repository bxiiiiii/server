#include "EventLoop.h"
#include <functional>

class Channel
{
public:
    typedef std::function<void()> EventCallBack;

    Channel(int fd, EventLoop* loop);
    ~Channel();

    void setReadCallBack(EventCallBack func){
        readCallBack = func;
    }
    void setWriteCallBack(EventCallBack func){
        writeCallBack = func;
    }
    void seterrorCallBack(EventCallBack func){
        errorCallBack = func;
    }

    void handleEvent();

private:
    void update();

    static const int NoneEvent;
    static const int ReadEvent;
    static const int WriteEvent;

    EventLoop *loop_;
    int fd_;
    int event_;
    int revent_;
    int index;

    EventCallBack readCallBack;
    EventCallBack writeCallBack;
    EventCallBack errorCallBack;
};