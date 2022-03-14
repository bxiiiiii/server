#include <string>

const int kSmallBuffer = 4000;
const int kLargeBuffer = 4000 * 1000;

template <int SIZE>
class FixedBuffer {
 public:
  FixedBuffer();
  ~FixedBuffer();
  void append(const char* buf, size_t len);

  const char* getdata();
  int getlen();
  void add(size_t len);
  intt avail();
  char* current();

  void reset();
  void bzero();

 private:
  const char* end() const;
  char data_[SIZE];
  char* cur;
};

class LogStream {
  typedef LogStream self;

 public:
  typedef FixedBuffer<kSmallBuffer> Buffer;

  self& operator<<(bool b);
  self& operator<<(short);
  self& operator<<(unsigned short);
  self& operator<<(int);
  self& operator<<(unsigned int);
  self& operator<<(long);
  self& operator<<(unsigned long);
  self& operator<<(long long);
  self& operator<<(unsigned long long);
  self& operator<<(const void*);
  self& operator<<(float b);
  self& operator<<(double);
  self& operator<<(char b);
  self& operator<<(const char* str);
  self& operator<<(const unsigned char* str);
  self& operator<<(const std::string& b);

  void append(const char* data, int len);
  const Buffer& getbuffer() const;
  void resetBuffer();

 private:
  static const int kMaxNumericSize = 48;
  Buffer buffer_;
  template <typename T>
  void formatInteger(T);
};

// class Fmt {
//  public:
//   template <typename T>
//   Fmt(const char* fmt, T val);

//   const char* getdata() const;
//   int getlen() const;

//  private:
//   char buf[32];
//   int len_;
// };