#include <iostream>

class Timestamp
{
public:
    Timestamp();
    static Timestamp now();

    static const int kMicroSecondsPerSecond = 1000 * 1000;
private:
    int64_t microSecondsSinceEpoch;
};