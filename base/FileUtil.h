#include <iostream>

static const int kBufferSize = 64 * 1024;
class ReadSmallFile {
 public:
  ReadSmallFile();
  ~ReadSmallFile();
  int readToBuffer(int* size);
  const char* getbuffer() const;

 private:
  int fd_;
  int err_;
  char buf[kBufferSize];
};

class AppendFile {
 public:
  AppendFile();
  ~AppendFile();
  void append(const char* logline, size_t len);
  void flush();
  size_t getwrittenBytes();

 private:
  size_t write(const char* logline, size_t len);

  FILE* fp_;
  char buffer_[kBufferSize];
  size_t writtenBytes_;
};

