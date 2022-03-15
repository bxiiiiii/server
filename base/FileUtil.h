#include <string>

static const int kBufferSize = 64 * 1024;
class ReadSmallFile {
 public:
  ReadSmallFile(std::string filename);
  ~ReadSmallFile();
  int readToBuffer(int* size);
  const char* getbuffer() const;

 private:
  int fd_;
  int err_;
  char buf_[kBufferSize];
};

class AppendFile {
 public:
  AppendFile(std::string filename);
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

