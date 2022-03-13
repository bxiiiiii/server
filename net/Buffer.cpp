#include "Buffer.h"

#include <errno.h>
#include <sys/uio.h>

Buffer::Buffer(int initialSize = kInitialSize) {}

int Buffer::readFd(int fd, int* Errno) {
  char extrabuf[655356];
  struct iovec vec[2];
  int writable = writableBytes();
  vec[0].iov_base = begin() + writeIndex_;
  vec[0].iov_len = writable;
  vec[1].iov_base = extrabuf;
  vec[1].iov_len = sizeof(extrabuf);
  int n = readv(fd, vec, 2);
  if (n < 0) {
    *Errno = errno;
  } else if (n <= writable) {
    writeIndex_ += n;
  } else {
    writeIndex_ = buffer_.size();
    append(extrabuf, n - writable);
  }
}

int Buffer::readableBytes() { return writeIndex_ - readIndex_; }

int Buffer::writableBytes() { return buffer_.size() - writeIndex_; }

int Buffer::prependableBytes() { return readIndex_; }

char* Buffer::begin() { return &*buffer_.begin(); }

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

void Buffer::makeSpace(size_t len)

{}
