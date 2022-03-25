#ifndef BASE_ASYNCLOGGING_H
#define BASE_ASYNCLOGGING_H

#include <condition_variable>
#include <memory>
#include <mutex>
#include <vector>
#include <functional>
#include <future>
#include "LogStream.h"
#include "Thread.h"

class AsyncLogging {
 public:
  AsyncLogging(std::string filename, int flushInterval = 3,
               int rollSize = 64 * 1024);
  ~AsyncLogging();

  void append(const char* log, size_t len);
  void start();
  void stop();

 private:
 void threadFunc();
//  def std::function<void()> ThreadFunc;
  
  typedef FixedBuffer<kLargeBuffer> Buffer;
  typedef std::vector<std::unique_ptr<Buffer>> BufferVector;
  typedef std::unique_ptr<Buffer> BufferPtr;

  const int flushInterval_;
  const size_t rollSize_;
  bool running_;
  std::string filename_;
  std::mutex mutex_;
  std::condition_variable cond_;
  std::promise<void> p_;
  Thread thread_;
  // ThreadFunc threadfunc_;
  BufferPtr currentBuffer_;
  BufferPtr nextBuffer_;
  BufferVector buffers_;
};

#endif