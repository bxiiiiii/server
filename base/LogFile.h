#ifndef BASE_LOGFILE_H
#define BASE_LOGFILE_H

#include <iostream>
#include <memory>
#include <mutex>

#include "FileUtil.h"

using namespace std;
class LogFile {
 public:
  LogFile(const string& filename, int rollSize, int flushInterval = 5,
          int maxCount = 1024);
  ~LogFile();

  void append(const char* log, int len);
  void flush();
  bool rollFile();

 private:
  string getLogFileName(const string& filename, time_t* now);
  void append_unlocked(const char* log, int len);
  const string filename_;
  // time_t lastFlush_;
  // time_t lastRoll_;
  // time_t startOfPeriod;
  int flustInterval_;
  int rollSize_;
  // int maxCount_;
  // int checkEveryN_;

  std::unique_ptr<AppendFile> file_;
  std::unique_ptr<std::mutex> mutex_;
  const static int kRollPerSeconds_ = 60 * 60 * 24;
};

#endif