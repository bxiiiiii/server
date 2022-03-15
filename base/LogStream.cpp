#include "LogStream.h"

const char digits[] = "9876543210123456789";
const char* zero = digits + 9;
const char digitsHex[] = "0123456789ABCDEF";

template <typename T>
size_t convert(char buf[], T value) {
  T i = value;
  char* p = buf;
  do {
    in lsd = static_cast<int>(i % 10);
    i /= 10;
    *p++ = zero[lsd];
  } while (i != 0)

      if (value < 0) {
    *p++ = '-';
  }
  *p = '\0';
  std::reverse(buf, p);

  return p - buf;
}

LogStream& LogStream::operator<<(bool b) {
  buffer_.append(b ? "1" : "0", 1);
  return *this;
}
LogStream& LogStream::operator<<(short) {}
LogStream& LogStream::operator<<(unsigned short) {}
LogStream& LogStream::operator<<(int) {}
LogStream& LogStream::operator<<(unsigned int) {}
LogStream& LogStream::operator<<(long) {}
LogStream& LogStream::operator<<(unsigned long) {}
LogStream& LogStream::operator<<(long long) {}
LogStream& LogStream::operator<<(unsigned long long) {}
LogStream& LogStream::operator<<(const void*) {}
LogStream& LogStream::operator<<(float b) {}
LogStream& LogStream::operator<<(double) {}
LogStream& LogStream::operator<<(char b) {}
LogStream& LogStream::operator<<(const char* str) {}
LogStream& LogStream::operator<<(const unsigned char* str) {}
LogStream& LogStream::operator<<(const std::string& b) {}

void LogStream::append(const char* data, int len) {}
const LogStream::Buffer& LogStream::getbuffer() const { return buffer_; }
void LogStream::resetBuffer() { buffer_.reset(); }

template <typename T>
void LogStream::formatInteger(T v) {
  if (buffer_.avail() >= kMaxNumericSize) {
    size_t len = convert(buffer_.current(), v);
    buffer_.add(len);
  }
}