#include "LogFile.h"

#include <time.h>

using namespace std;

LogFile::LogFile(const string& filename, int rollSize, int flushInterval,
                 int maxCount)
    : filename_(filename),
      rollSize_(rollSize),
      flustInterval_(flushInterval)
    //   maxCount_(maxCount),
    //   lastFlush_(0),
    //   lastRoll_(0) 
      {
        rollFile();
      }

LogFile::~LogFile() = default;

string LogFile::getLogFileName(const string& filename, time_t* now) {
  string basename;
  basename.reserve(filename.size() + 64);
  basename = filename;

  char time_str[32];
  struct tm t;
  *now = time(0);
  gmtime_r(now, &t);
  strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", &t);
  basename += time_str;

  basename += ".log";

  return basename;
}

void LogFile::append(const char* logline, int len) {
  if (mutex_) {
    std::unique_lock<std::mutex> lock(*mutex_);
    append_unlocked(logline, len);
  } else {
    append_unlocked(logline, len);
  }
}

void LogFile::flush()
{
    if(mutex_) {
        std::unique_lock<std::mutex> lock(*mutex_);
        file_->flush();
    }else {
        file_->flush();
    }
}

void LogFile::append_unlocked(const char* log, int len) {
  file_->append(log, len);
//   if (file_->getwrittenBytes() > rollSize_) {
//     rollFile();
//   } else {
//     ++maxCount_;
//     if (maxCount_ >= checkEveryN_) {
//       maxCount_ = 0;
//       time_t now = ::time(nullptr);
//       time_t thisPeriod = now / kRollPerSeconds_ * kRollPerSeconds_;
//       if (thisPeriod != startOfPeriod) {
//         rollFile();
//       } else if (now - lastFlush_ > flustInterval_) {
//         lastFlush_ = now;
//         file_->flush();
//       }
//     }
//   }
    //if(file_->getwrittenBytes() >= rollSize_){
        rollFile();
    //}
}

bool LogFile::rollFile() {
  time_t now = 0;
  std::string filename = getLogFileName(filename_, &now);
//   time_t start = now / kRollPerSeconds_ * kRollPerSeconds_;
//   if (now > lastRoll_) {
//     lastRoll_ = now;
//     lastFlush_ = now;
//     startOfPeriod = start;
//     file_.reset(new AppendFile(filename));
//     return true;
//   }
    file_ = std::make_unique<AppendFile>(filename);

  return true;
}