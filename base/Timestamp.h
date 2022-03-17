#include <iostream>

class Timestamp
{
public:
    Timestamp();
    Timestamp(int64_t microSeconds);
    static Timestamp now();

    static const int kMicroSecondsPerSecond = 1000 * 1000;
private:
    int64_t microSecondsSinceEpoch;
};