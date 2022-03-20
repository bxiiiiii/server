#include "Buffer.h"

#include <errno.h>
#include <sys/uio.h>
#include <unistd.h>

#include <string>
#include "../base/Logging.h"

void Buffer::swap(Buffer& rhs) {
  buffer_.swap(rhs.buffer_);
  std::swap(readIndex_, rhs.readIndex_);
  std::swap(writeIndex_, rhs.writeIndex_);
}

int Buffer::readableBytes() { return writeIndex_ - readIndex_; }

int Buffer::writableBytes() { return buffer_.size() - writeIndex_; }

int Buffer::prependableBytes() { return readIndex_; }

const char* Buffer::peek() { return begin() + readIndex_; }

char* Buffer::begin() { return &*buffer_.begin(); }

void Buffer::retrieve(size_t len) {
  if (len < readableBytes()) {
    readIndex_ += len;
  } else {
    retrieveAll();
  }
}

void Buffer::retrieveAll() {
  readIndex_ = kPrepend;
  writeIndex_ = kPrepend;
}

std::string Buffer::retrieveAllAsString() {
  return retrieveAsString(readableBytes());
}
std::string Buffer::retrieveAsString(size_t len) {
  std::string result(peek(), len);
  retrieve(len);
  return result;
}

void Buffer::append(const char* data, int len) {
  ensureWritableBytes(len);
  std::copy(data, data + len, beginWrite());
  hasWritten(len);
}

void Buffer::ensureWritableBytes(size_t len) {
  if (writableBytes() < len) {
    makeSpace(len);
  }
}
char* Buffer::beginWrite() { return begin() + writeIndex_; }

void Buffer::hasWritten(size_t len) { writeIndex_ += len; }

void Buffer::makeSpace(size_t len) {
  if (writableBytes() + prependableBytes() < len + kPrepend) {
    buffer_.resize(writeIndex_ + len);
  } else {
    int readable = readableBytes();
    std::copy(begin() + readIndex_, begin() + writeIndex_, begin() + kPrepend);
    readIndex_ = kPrepend;
    writeIndex_ = readIndex_ + readable;
  }
}

int Buffer::readFd(int fd, int* Errno) {
  char extrabuf[65536];
  struct iovec vec[2];
  int writable = writableBytes();
  vec[0].iov_base = begin() + writeIndex_;
  vec[0].iov_len = writable;
  vec[1].iov_base = extrabuf;
  vec[1].iov_len = sizeof extrabuf;
  const int iovcnt = (writable < sizeof extrabuf) ? 2 : 1;
  size_t n = readv(fd, vec, iovcnt);
  LOG_DEBUG << n;
  if (n < 0) {
    *Errno = errno;
  } else if (n <= writable) {
    writeIndex_ += n;
  } else {
    writeIndex_ = buffer_.size();
    append(extrabuf, n - writable);
  }
  //int n = read(fd, begin() + writeIndex_, writableBytes());
  return n;
}