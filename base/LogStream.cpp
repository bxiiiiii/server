#include "LogStream.h"
#include <string>
#include <algorithm>
#include <cstring>

const char digits[] = "9876543210123456789";
const char* zero = digits + 9;
const char digitsHex[] = "0123456789ABCDEF";

template <typename T>
size_t convert(char buf[], T value) {
  T i = value;
  char* p = buf;
  do {
    int lsd = static_cast<int>(i % 10);
    i /= 10;
    *p++ = zero[lsd];
  } while (i != 0);

      if (value < 0) {
    *p++ = '-';
  }
  *p = '\0';
  std::reverse(buf, p);

  return p - buf;
}

size_t convertHex(char buf[], uintptr_t value)
{
  uintptr_t i = value;
  char *p = buf;

  do{
    int isd = static_cast<int> (i % 16);
    i /= 16;
    *p++ = digitsHex[isd];
  }while(i != 0);

  *p = '\0';
  std::reverse(buf, p);

  return p - buf;
}

LogStream& LogStream::operator<<(bool b) {
  buffer_.append(b ? "1" : "0", 1);
  return *this;
}
LogStream& LogStream::operator<<(short v) {
  *this << static_cast<int>(v);
  return *this;
}
LogStream& LogStream::operator<<(unsigned short v) {
  *this << static_cast<unsigned int>(v);
  return *this;
}
LogStream& LogStream::operator<<(int v) {
  formatInteger(v);
  return *this;
}
LogStream& LogStream::operator<<(unsigned int v) {
  formatInteger(v);
  return *this;
}
LogStream& LogStream::operator<<(long v) {
  formatInteger(v);
  return *this;
}
LogStream& LogStream::operator<<(unsigned long v) {
  formatInteger(v);
  return *this;
}
LogStream& LogStream::operator<<(long long v) {
  formatInteger(v);
  return *this;
}
LogStream& LogStream::operator<<(unsigned long long v) {
  formatInteger(v);
  return *this;
}
LogStream& LogStream::operator<<(const void* p) 
{
  auto v = reinterpret_cast<uintptr_t>(p);
  if(buffer_.avail() >= kMaxNumericSize){
    char* buf = buffer_.current();
    buf[0] = '0';
    buf[1] = 'x';
    size_t len = convertHex(buf + 2, v);
    buffer_.add(len + 2);
  }

  return *this;
}
LogStream& LogStream::operator<<(float b) {
  *this <<static_cast<double>(b);
  return *this;
}
LogStream& LogStream::operator<<(double v) {
  if(buffer_.avail() >= kMaxNumericSize){
    int len = snprintf(buffer_.current(), kMaxNumericSize, "%.12g", v);
    buffer_.add(len);
  }
  return *this;
}
LogStream& LogStream::operator<<(char b) {
  buffer_.append(&b, 1);
  return *this;
}
LogStream& LogStream::operator<<(const char* b) 
{
    if(b){
    buffer_.append(b, strlen(b));
  } else {
    buffer_.append("null", 6);
  }

  return *this;
}
LogStream& LogStream::operator<<(const unsigned char* str) {
  return operator<<(reinterpret_cast<const char*>(str));
}
LogStream& LogStream::operator<<(const std::string& b) 
{
  buffer_.append(b.c_str(), b.size());
  return *this;
}

void LogStream::append(const char* data, int len) { buffer_.append(data, len); }
const LogStream::Buffer& LogStream::getbuffer() const { return buffer_; }
void LogStream::resetBuffer() { buffer_.reset(); }

template <typename T>
void LogStream::formatInteger(T v) {
  if (buffer_.avail() >= kMaxNumericSize) {
    size_t len = convert(buffer_.current(), v);
    buffer_.add(len);
  }
}