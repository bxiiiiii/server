#include "Logging.h"

#include <chrono>

Logger::LogLevel initLogLevel() { return Logger::DEBUG; }
Logger::LogLevel g_logLevel = initLogLevel();

const char* LogLevelName[Logger::NUM_LOG_LEVELS] = {"TRACE", "DEBUF", "INFO",
                                                    "WARN",  "ERROR", "FATAL"};
class T {
 public:
  T(const char* str, unsigned len) : str_(str), len_(len) {}

  const char* str_;
  const unsigned len_;
};

void defaultOutput(const char* msg, int len) {
  size_t n = fwrite(msg, 1, len, stdout);
  // TODO
}

void defaultFlush() { fflush(stdout); }

Logger::OutputFunc g_output = defaultOutput;
Logger::FlushFunc g_flush = defaultFlush;

Logger::Logger(SourceFile file, int line) : impl_(INFO, 0, file, line) {}
Logger::Logger(SourceFile file, int line, LogLevel level)
    : impl_(level, 0, file, line) {}
Logger::Logger(SourceFile file, int line, LogLevel level, const char* func)
    : impl_(level, 0, file, line) {
  impl_.stream_ << func << ' ';
}
Logger::Logger(SourceFile file, int line, bool toAbort)
    : impl_(toAbort ? FATAL : ERROR, errno, file, line) {}

Logger::~Logger() {
  impl_.finish();
  const LogStream::Buffer& buf(getstream().getbuffer());
  g_output(buf.getdata(), buf.getlen());
  if (impl_.level_ == FATAL) {
    g_flush();
    abort();
  }
}

LogStream& Logger::getstream() { return impl_.stream_; }

void Logger::setLogLevel(LogLevel level) { g_logLevel = level; }
void Logger::setOutput(OutputFunc func) { g_output = func; }
void Logger::setFlush(FlushFunc func) { g_flush = func; }

Logger::Impl::Impl(LogLevel level, int old_error, const SourceFile& file,
                   int line)
    : time_(Timestamp::now()),
      stream_(),
      level_(level),
      line_(line),
      filename_(file) {
        FormatAndPrintTime(time_);
      }

void Logger::Impl::finish() {
  stream_ << "-" << filename_.data_ << ":" << line_ << '\n';
}

void Logger::Impl::FormatAndPrintTime(Timestamp time) {
  int64_t milli = time.getms()/1000 + (int64_t)8 * 60 * 60 * 1000;
  auto mTime = std::chrono::milliseconds(milli);
  auto tp = std::chrono::time_point<std::chrono::system_clock,
                                    std::chrono::milliseconds>(mTime);
  auto tt = std::chrono::system_clock::to_time_t(tp);
  std::tm* now = std::gmtime(&tt);
  stream_ << now->tm_year + 1900 << '-' << now->tm_mon + 1 << '-'
          << now->tm_mday << ' ' << now->tm_hour << ':' << now->tm_min << ':'
          << now->tm_sec << ' ';
}