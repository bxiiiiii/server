#ifndef BASE_LOGGING_H
#define BASE_LOGGING_H

#include "LogStream.h"
#include "Timestamp.h"

class Logger {
 public:
  enum LogLevel {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    NUM_LOG_LEVELS,
  };

  class SourceFile {
   public:
    template <int N>
    SourceFile(const char (&arr)[N]) : data_(arr), size_(N - 1) {
      const char* slash = strrchr(data_, '/');
      if (slash) {
        data_ = slash + 1;
        size_ -= static_cast<int>(data_ - arr);
      }
    }

    explicit SourceFile(const char* filename) : data_(filename) {
      const char* slash = strrchr(filename, '/');
      if (slash) {
        data_ = slash + 1;
        size_ = static_cast<int>(strlen(data_));
      }
    }

    const char* data_;
    int size_;
  };

  Logger(SourceFile file, int line);
  Logger(SourceFile file, int line, LogLevel level);
  Logger(SourceFile file, int line, LogLevel level, const char* func);
  Logger(SourceFile file, int line, bool toAbort);
  ~Logger();

  LogStream& getstream() { return impl_.stream_; }

  static LogLevel getloglevel();
  static void setLogLevel(LogLevel level);

  typedef void (*OutputFunc)(const char* msg, int len);
  typedef void (*FlushFunc)();
  static void setOutput(OutputFunc);
  static void setFlush(FlushFunc);

 private:
  class Impl {
   public:
    typedef Logger::LogLevel LogLevel;
    Impl(LogLevel level, int old_error, const SourceFile& file, int line);
    void finish();

    Timestamp time_;
    LogStream stream_;
    LogLevel level_;
    int line_;
    SourceFile filename_;
  };

  Impl impl_;
};

extern Logger::LogLevel g_logLevel;
inline Logger::LogLevel Logger::getloglevel() { return g_logLevel; }
#define LOG_TRACE                             \
  if (Logger::getloglevel() <= Logger::TRACE) \
  Logger(__FILE__, __LINE__, Logger::TRACE, __func__).getstream()
#define LOG_DEBUG                             \
  if (Logger::getloglevel() <= Logger::DEBUG) \
  Logger(__FILE__, __LINE__, Logger::DEBUG, __func__).getstream()
#define LOG_INFO                             \
  if (Logger::getloglevel() <= Logger::INFO) \
  Logger(__FILE__, __LINE__, Logger::INFO, __func__).getstream()
#define LOG_WARN Logger(__FILE__, __LINE__, Logger::WARN).getstream()
#define LOG_ERROR Logger(__FILE__, __LINE__, Logger::ERROR).getstream()
#define LOG_FATAL Logger(__FILE__, __LINE__, Logger::FATAL).getstream()
#define LOG_SYSERR Logger(__FILE__, __LINE__, false).gerstream()
#define LOG_SYSFATAL Logger(__FILE__, __LINE__, false).gerstream()

#endif