#include <condition_variable>
#include <memory>
#include <mutex>
#include <vector>
#include <thread>
#include <functional>

#include "LogStream.h"

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
  std::thread thread_;
  // ThreadFunc threadfunc_;
  BufferPtr currentBuffer_;
  BufferPtr nextBuffer_;
  BufferVector buffers_;
};