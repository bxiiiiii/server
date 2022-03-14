#include "LogStream.h"

const char digits[] = "9876543210123456789";
const char* zero = digits + 9;
const char digitsHex[] = "0123456789ABCDEF";

template <typename T>
size_t convert(char buf[], T value) {}

template <typename T>
void LogStream::formatInteger(T v) {
  if (buffer_.avail() >= kMaxNumericSize) {
    size_t len = convert(buffer_.current(), v);
    buffer_.add(len);
  }
}

LogStream& LogStream::operator<<(short b)
{
}
LogStream& LogStream::operator<<(bool b){}
LogStream& LogStream::operator<<(short){}
LogStream& LogStream::operator<<(unsigned short){}
LogStream& LogStream::operator<<(int){}
LogStream& LogStream::operator<<(unsigned int){}
LogStream& LogStream::operator<<(long){}
LogStream& LogStream::operator<<(unsigned long){}
LogStream& LogStream::operator<<(long long){}
LogStream& LogStream::operator<<(unsigned long long){}
LogStream& LogStream::operator<<(const void*){}
LogStream& LogStream::operator<<(float b){}
LogStream& LogStream::operator<<(double){}
LogStream& LogStream::operator<<(char b){}
LogStream& LogStream::operator<<(const char* str){}
LogStream& LogStream::operator<<(const unsigned char* str){}
LogStream& LogStream::operator<<(const std::string& b){}