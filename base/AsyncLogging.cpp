#include "AsyncLogging.h"

#include <chrono>
#include <iostream>

#include "LogFile.h"

AsyncLogging::AsyncLogging(std::string filename, int flushInterval,
                           int rollSize)
    : flushInterval_(flushInterval),
      rollSize_(rollSize),
      running_(false),
      filename_(filename),
      thread_(std::bind(&AsyncLogging::threadFunc, this)),
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
  std::future<void> f = p_.get_future();
  running_ = true;
  thread_.start();
  f.get();
}
void AsyncLogging::stop() {
  running_ = false;
  cond_.notify_one();
  thread_.join();
}

void AsyncLogging::threadFunc() {
  p_.set_value();
  LogFile output(filename_, rollSize_, false);
  BufferPtr newBuffer1(new Buffer);
  BufferPtr newBuffer2(new Buffer);
  newBuffer1->bzero();
  newBuffer2->bzero();
  BufferVector buffersToWrite;
  buffersToWrite.reserve(16);
  while (running_) {
    {
      printf("%p\n", currentBuffer_.get());
      printf("%p\n", nextBuffer_.get());
      printf("%p\n", newBuffer1.get());
      printf("%p\n", newBuffer2.get());
      std::unique_lock<std::mutex> lock(mutex_);
      if (buffers_.empty()) {
        cond_.wait_for(lock, std::chrono::seconds(flushInterval_));
      }
      buffers_.push_back(std::move(currentBuffer_));
      currentBuffer_ = std::move(newBuffer1);
      buffersToWrite.swap(buffers_);
      if (!nextBuffer_) {
        nextBuffer_ = std::move(newBuffer2);
      }
    }

    if (buffersToWrite.size() > 25) {
      buffersToWrite.erase(buffersToWrite.begin() + 2, buffersToWrite.end());
    }
    std::cout << buffersToWrite.size() << std::endl;
    for (auto& buffer : buffersToWrite) {
      std::cout << buffer->getlen() << std::endl;
      output.append(buffer->getdata(), buffer->getlen());
    }
    if (buffersToWrite.size() > 2) {
      buffersToWrite.resize(2);
    }
    if (!newBuffer1) {
      newBuffer1 = std::move(buffersToWrite.back());
      buffersToWrite.pop_back();
      newBuffer1->reset();
    }
    if (!newBuffer2) {
      newBuffer2 = std::move(buffersToWrite.back());
      buffersToWrite.pop_back();
      newBuffer2->reset();
    }
    buffersToWrite.clear();
    output.flush();
  }
  output.flush();
}