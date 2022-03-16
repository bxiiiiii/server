#include "AsyncLogging.h"
#include "LogFile.h"
#include <chrono>

AsyncLogging::AsyncLogging(std::string filename, int flushInterval = 3,
                           int rollSize = 64 * 1024)
    : flushInterval_(flushInterval),
      rollSize_(rollSize),
      running_(false),
      filename_(filename),
      thread_(),
      mutex_(),
      // cond_(mutex_),
      currentBuffer_(new Buffer),
      nextBuffer_(new Buffer) {
  currentBuffer_->bzero();
  nextBuffer_->bzero();
  buffers_.reserve(16);
}

AsyncLogging::~AsyncLogging() {
  if (running_) {
    stop();
  }
}

void AsyncLogging::append(const char* log, size_t len) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (currentBuffer_->avail() > len) {
    currentBuffer_->append(log, len);
  } else {
    buffers_.push_back(std::move(currentBuffer_));
    if (nextBuffer_) {
      currentBuffer_ = std::move(nextBuffer_);
    } else {
      currentBuffer_.reset(new Buffer);
    }
    currentBuffer_->append(log, len);
    cond_.notify_one();
  }
}
void AsyncLogging::start() {
  running_ = true;
  thread_ = std::thread(&AsyncLogging::threadFunc, this);
}
void AsyncLogging::stop() {
  running_ = false;
  cond_.notify_one();
  thread_.join();
}

void AsyncLogging::threadFunc() {
  LogFile output(filename_, rollSize_, false);
  BufferPtr newBuffer1(new Buffer);
  BufferPtr newBuffer2(new Buffer);
  newBuffer1->bzero();
  newBuffer2->bzero();
  BufferVector buffersToWrite;
  buffersToWrite.reserve(16);
  while (running_) {
    {
      {
        std::unique_lock<std::mutex> lock(mutex_);
        if(buffers_.empty()){
          cond_.wait_for(lock, std::chrono::seconds(flushInterval_));  
        }
        buffers_.push_back(std::move(currentBuffer_));
        currentBuffer_ = std::move(newBuffer1);
        buffersToWrite.swap(buffers_);
        if(!nextBuffer_){
          nextBuffer_ = std::move(newBuffer2);
        }
      }

      if(buffersToWrite.size() > 25){
        buffersToWrite.erase(buffersToWrite.begin() + 2, buffersToWrite.end());
      }

      for(auto& buffer : buffersToWrite) {
        output.append(buffer->getdata(), buffer->getlen());
      }
      if(buffersToWrite.size() > 2){
        buffersToWrite.resize(2);
      }
      if(!newBuffer1){
        newBuffer1 = std::move(buffersToWrite.back());
        buffersToWrite.pop_back();
        newBuffer1.reset();
      }
      if(!newBuffer2){
        newBuffer2 = std::move(buffersToWrite.back());
        buffersToWrite.pop_back();
        newBuffer2.reset();
      }
      buffersToWrite.clear();
      output.flush();
    }
  }
}