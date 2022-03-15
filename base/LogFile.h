#include <iostream>

using namespace std;
class LogFile
{
public:
    LogFile(const string& filename,
            long rollSize,
            int flushInterval = 3,
            int maxCount = 1024);
    ~LogFile();

    void append(char* log, int len);
    void flush();
    bool rollFile();

private:
    string getLogFileName(const string& filename, time_t *now);

    const string filename_;
    time_t lastFlush_;
    time_t lastRoll_;
    time_t flustInterval_;
    time_t rollSize_;
    int maxCount_;
};