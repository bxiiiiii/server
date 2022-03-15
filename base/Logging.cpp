#include "Logging.h"

Logger::LogLevel initLogLevel() { return Logger::INFO; }
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

Logger::OutputFunc g_output;
Logger::FlushFunc g_flush;

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
Logger::LogLevel Logger::getloglevel() { return g_logLevel; }

void Logger::setLogLevel(LogLevel level) { g_logLevel = level; }
void Logger::setOutput(OutputFunc func) { g_output = func; }
void Logger::setFlush(FlushFunc func) { g_flush = func; }

Logger::Impl::Impl(LogLevel level, int old_error, const SourceFile& file,
                   int line)
    : time_(Timestamp::now()),
      stream_(),
      level_(level),
      line_(line),
      filename_(file) {}

void Logger::Impl::finish() {
  stream_ << "-" << filename_ << ":" << line_ << '\n';
}