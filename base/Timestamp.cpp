#include "Timestamp.h"

#include <sys/time.h>

    Timestamp::Timestamp()
    :microSecondsSinceEpoch(0)
    {
    }
    Timestamp::Timestamp(int64_t microSeconds)
    :microSecondsSinceEpoch(microSeconds)
    {

    }
    Timestamp Timestamp::now()
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        int64_t seconds = tv.tv_sec;
        return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
    }