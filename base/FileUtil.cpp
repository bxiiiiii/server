#include "FileUtil.h"

#include <fcntl.h>
#include <unistd.h>

ReadSmallFile::ReadSmallFile(std::string filename)
    : fd_(open(filename.data(), O_RDONLY | O_CLOEXEC)), err_(0) {
  buf_[0] = '\0';
  if (fd_ < 0) {
    err_ = errno;
  }
}
ReadSmallFile::~ReadSmallFile() {
  if (fd_ >= 0) {
    ::close(fd_);
  }
}
int ReadSmallFile::readToBuffer(int* size) {
  int err = err_;
  if (fd_ >= 0) {
    ssize_t n = ::pread(fd_, buf_, sizeof(buf_) - 1, 0);
    if (n >= 0) {
      if (size) {
        *size = static_cast<ssize_t>(n);
      }
      buf_[n] = '\0';
    } else {
      err = errno;
    }
  }

  return err;
}
const char* ReadSmallFile::getbuffer() const { return buf_; }

AppendFile::AppendFile(std::string filename)
    : fp_(fopen(filename.data(), "ae")), writtenBytes_(0) {
  setbuffer(fp_, buffer_, sizeof(buffer_));
}

AppendFile::~AppendFile() { fclose(fp_); }

void AppendFile::append(const char* logline, size_t len) {
  size_t written = 0;

  while (written != len) {
    size_t remain = len - written;
    size_t n = write(logline + written, remain);
    if (n != remain) {
      int err = ferror(fp_);
      if (err) {
        fprintf(stderr, "write file failed.");
        break;
      }
    }
    written += n;
  }
}
void AppendFile::flush() { fflush(fp_); }
size_t AppendFile::getwrittenBytes() { return writtenBytes_; }
size_t AppendFile::write(const char* logline, size_t len) {
  fwrite_unlocked(logline, 1, len, fp_);
}