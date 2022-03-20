#ifndef BASE_TIMESTAMP_H
#define BASE_TIMESTAMP_H
#include <iostream>

class Timestamp
{
public:
    Timestamp();
    Timestamp(int64_t microSeconds);
    static Timestamp now();
    int64_t getms() { return microSecondsSinceEpoch; };

    static const int kMicroSecondsPerSecond = 1000 * 1000;
private:
    int64_t microSecondsSinceEpoch;
};

#endif