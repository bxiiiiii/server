#include "AsyncLogging.h"


  AsyncLogging::AsyncLogging(std::string filename, int flushInterval = 3, int rollSize = 64 * 1024)
  : flushInterval_(flushInterval),
    rollSize_(rollSize),
    running_(false),
    filename_(filename),
    // thread_
    mutex_(),
    cond_(),
    currentBuffer_(new Buffer),
    nextBuffer_(new Buffer)
  {
      currentBuffer_->bzero();
      nextBuffer_->bzero();
      buffers_.reserve(16);
  }

  AsyncLogging::~AsyncLogging()
  {

  }

    void AsyncLogging::append(const char* log, size_t len)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if(currentBuffer_->avail() > len){
            currentBuffer_->append(log, len);
        } else {
            buffers_.push_back()
        }
    }
    void AsyncLogging::start()
    {

    }
    void AsyncLogging::stop()
    {

    }