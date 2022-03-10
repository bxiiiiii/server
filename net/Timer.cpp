#include <iostream>
#include <functional>

#include "Timestamp.h"


class Timer
{
    typedef std::function<void()> TimeCallBack;

    Timer(TimeCallBack callback, Timestamp when, int rea);

private:
    TimeCallBack callback_;
}