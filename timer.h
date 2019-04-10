#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>
#include <utility>
#include <string>

class Timer
{
public:
    Timer();
    static int timerBegin();
    static long timerEnd(int id);
    static std::string timeIn(long seconds);
    static std::string timeFrom(long seconds);

    /// unix time in secs,usecs
    typedef std::pair<long,long> time;

    static long diffTimes(time one, time two);
    static time getCurrentTime();
};

std::tuple<short, short, short> timeSplit(long seconds);
unsigned long getTime(long baseSeconds);
unsigned long getBaseSeconds();

#endif // TIMER_H
