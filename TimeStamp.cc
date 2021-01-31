/*
	* File     : TimeStamp.cc
	* Author   : *
	* Mail     : *
	* Creation : Sun 31 Jan 2021 01:51:53 PM CST
*/

#include <bits/types/struct_tm.h>
#include <cstdio>
#include <iostream>
#include "TimeStamp.h"
#include <sys/time.h>
#include <inttypes.h>

std::string TimeStamp::to_string() const {
    char buf[32] = {'\0'};
    int64_t seconds = micro_seconds_since_epoch_ / kMicroSecondsPerSecond;
    int64_t micro_seconds = micro_seconds_since_epoch_ % kMicroSecondsPerSecond;

    snprintf(buf, sizeof(buf), "%" PRId64 ".%06" PRId64 "", seconds, micro_seconds);
    return buf;
}

std::string TimeStamp::to_formatted_string(bool show_microseconds) const {
    char buf[64] = {'\0'};
    time_t seconds = static_cast<time_t>(micro_seconds_since_epoch_ / kMicroSecondsPerSecond);
    struct tm t;
    gmtime_r(&seconds, &t);

    if (show_microseconds) {
        int micro_seconds = static_cast<int>(micro_seconds_since_epoch_ % kMicroSecondsPerSecond);
        snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
                 t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
                 t.tm_hour, t.tm_min, t.tm_sec, micro_seconds);

    }
    else {
        snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d",
                 t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
                 t.tm_hour, t.tm_min, t.tm_sec);
    }
    return buf;
}

TimeStamp TimeStamp::now() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t seconds = tv.tv_sec;
    return TimeStamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}

