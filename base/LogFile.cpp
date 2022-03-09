#include "LogFile.h"
#include <time.h>

using namespace std;

LogFile::LogFile(const string& filename,
                long rollSize,
                int flushInterval = 3,
                int maxCount = 1024)
    : filename_(filename),
        rollSize_(rollSize),
        flustInterval_(flushInterval),
        maxCount_(maxCount),
        lastFlush_(0),
        lastRoll_(0){ }

LogFile::~LogFile() = default;


string LogFile::getLogFileName(const string& filename, time_t* now)
{
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