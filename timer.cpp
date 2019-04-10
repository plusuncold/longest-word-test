#include "timer.h"

#include <memory>
#include <map>
#include <ctime>
#include <iostream>

static std::map<int,Timer::time> beginningTimes;
static int maxID = 0;

static struct timeval timeStruct;

const long ONE_SECOND_IN_MICROSECONDS = 1000000;

Timer::Timer() {}

int Timer::timerBegin() {
    beginningTimes[maxID] = getCurrentTime();

    return maxID++;
}

long Timer::timerEnd(int id) {    
    time begin = beginningTimes[id];
    
    auto diff = diffTimes(begin, getCurrentTime());
    beginningTimes.erase(id);
    return diff;

}

Timer::time Timer::getCurrentTime() {
    time tim;
    gettimeofday(&timeStruct, nullptr);
    tim.first = timeStruct.tv_sec;
    tim.second = timeStruct.tv_usec;
    return tim;
}

/**
   ret long difference in time in microseconds
 */
long Timer::diffTimes(time one, time two) {
    long diffSecs = two.first - one.first;
    diffSecs *= ONE_SECOND_IN_MICROSECONDS;
    long diffUSecs = two.second - one.second;
    return (diffSecs + diffUSecs);
}

std::string Timer::timeIn(long seconds) {
    std::time_t t = std::time(nullptr);
    struct tm *now = std::localtime(&t);
    short nowHour = now->tm_hour;
    short nowMin = now->tm_min;
    bool tomorrow = false;
    short diffWholeHours; short diffWholeMins;
    std::tie(diffWholeHours,diffWholeMins,seconds) = timeSplit(seconds);

    nowMin += diffWholeMins;
    if (nowMin >= 60) {
        diffWholeHours++;
        nowMin -= 60;
    }

    nowHour += diffWholeHours;
    if (nowHour >= 24) {
        tomorrow = true;
        nowHour -= 24;
    }

    if (nowHour >= 24) {
        if (nowHour < 48) return std::string("Day After Tomorrow");
        return std::string("After the Day After Tommorrow");
    }

    std::string retString, hourString, minString;

    if (nowHour < 10) hourString += '0';
    if (nowMin < 10) minString += '0';
    hourString += std::to_string(nowHour);
    minString += std::to_string(nowMin);

    if (tomorrow) retString += "Tomorrow ";
    retString +=  hourString + ":" + minString;

    return retString;
}

std::string Timer::timeFrom(long seconds) {
    std::time_t t = std::time(nullptr);
    struct tm *now = std::localtime(&t);

    short nowHour = now->tm_hour;
    short nowMin = now->tm_min;
    bool yesterday = false;

    short diffWholeHours; short diffWholeMins;
    std::tie(diffWholeHours,diffWholeMins,seconds) = timeSplit(seconds);

    nowMin -= diffWholeMins;
    if (nowMin < 0) {
        diffWholeHours++;
        nowMin += 60;
    }

    nowHour -= diffWholeHours;
    if (nowHour < 0) {
        yesterday = true;
        nowHour += 24;
    }

    if (nowHour < 0) return std::string("Day before yesterday");

    std::string retString, hourString, minString;

    if (nowHour < 10) hourString += '0';
    if (nowMin < 10) minString += '0';
    hourString += std::to_string(nowHour);
    minString += std::to_string(nowMin);

    if (yesterday) retString += "Yesterday ";
    retString +=  hourString + ":" + minString;

    return retString;
}

std::tuple<short, short, short> timeSplit(long seconds) {
    if (seconds < 0) {
	std::cout << seconds  << " seconds is negative" << std::endl;
	return std::make_tuple(0,0,0);
    }
    
    short wholeHours = seconds / 3600;
    seconds -= wholeHours * 3600;
    short wholeMins = seconds / 60;
    seconds -= wholeMins * 60;

    return std::make_tuple(wholeHours, wholeMins, seconds);
}

unsigned long getTime(long baseSeconds) {
    gettimeofday(&timeStruct,nullptr);
    long retTime = timeStruct.tv_usec;
    long secondsDifference = timeStruct.tv_sec - baseSeconds;
    if (secondsDifference > 0) {
	retTime += secondsDifference * ONE_SECOND_IN_MICROSECONDS;
    }
    return retTime;
}

unsigned long getBaseSeconds() {
    gettimeofday(&timeStruct,nullptr);
    return timeStruct.tv_sec;
}
