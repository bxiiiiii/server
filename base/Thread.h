#ifndef BASE_THREAD_H
#define BASE_THREAD_H

#include <functional>
#include <thread>

class Thread {
 public:
  typedef std::function<void()> ThreadFunc;
  Thread(ThreadFunc);
  ~Thread();

  void start();
  bool started() { return started_; }
  pid_t gettid() {return tid_;}
 private:
  bool started_;
  pid_t tid_;
  std::unique_ptr<std::thread> thread_;
  ThreadFunc func_;
};

#endif