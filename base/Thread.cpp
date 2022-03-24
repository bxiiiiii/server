#include "Thread.h"
#include "../base/Logging.h"

#include <syscall.h>
#include <unistd.h>
// namespace CurrentThread {
// thread_local int t_cachedTid = 0;
// thread_local std::string t_tidString;
// }  // namespace CurrentThread

Thread::Thread(ThreadFunc func)
    : started_(false), tid_(0), func_(std::move(func)) {}

Thread::~Thread() {}

pid_t gettid() { return syscall(SYS_gettid); }

class ThreadData {
 public:
  typedef std::function<void()> ThreadFunc;
  ThreadData(ThreadFunc& func, std::string& name, pid_t* tid, std::promise<void> &latch)
      : func_(func), name_(name), tid_(tid), latch_(latch){};
  ~ThreadData() = default;

  ThreadFunc func_;
  std::string name_;
  pid_t* tid_;
  std::promise<void> &latch_;
};

void* runInThread(void* arg) {
  LOG_DEBUG << "";
  ThreadData* data = static_cast<ThreadData*>(arg);
  *data->tid_ = gettid();
    data->latch_.set_value();
  data->func_();

  return NULL;
}

void Thread::start() {
  LOG_DEBUG <<"";
  std::future<void> latch = latch_.get_future();
  started_ = true;
  ThreadData* data = new ThreadData(func_, name_, &tid_, latch_);
  if (pthread_create(&pthreadId_, NULL, &runInThread, data)) {
    started_ = false;
    delete data;
    LOG_DEBUG << "thread create failed";
  } else {
    latch.get();
  }
}

int Thread::join() { return pthread_join(pthreadId_, NULL); }