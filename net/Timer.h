#include <chrono>
#include <functional>
#include <iostream>

#include "CallBacks.h"
// #include "Timestamp.h"

class Timer {
 public:
  using TimeUnit = std::chrono::nanoseconds;
  using TimePoint = std::chrono::steady_clock::time_point;
  Timer(TimeCallBack callback, TimePoint when, TimeUnit interval)
      : callback_(std::move(callback)),
        expiration_(when),
        interval_(interval),
        repeat_(interval.count() > 0),
        delete_(false),
        sequence_(s_numCreated_++){};

  void run() const { callback_(); }

  TimePoint getExpiration() const { return expiration_; }
  bool getRepeat() const { return repeat_; }
  bool getDelete() const { return delete_; }
  int64_t getSequence() const { return sequence_; }
  void restart(const Timer::TimePoint& now) { expiration_ = now + interval_; }
  void setDelete(bool on) { delete_ = on; }


 private:
  TimeCallBack callback_;
  TimePoint expiration_;
  TimeUnit interval_;
  bool repeat_;
  bool delete_;
  int64_t sequence_;
  static int64_t s_numCreated_;
};