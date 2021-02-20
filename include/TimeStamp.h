/*
	* File     : TimeStamp.h
	* Author   : *
	* Mail     : *
	* Creation : Sun 31 Jan 2021 01:51:50 PM CST
*/

#ifndef _TIMESTAMP_H
#define _TIMESTAMP_H
#include <bits/stdint-intn.h>
#include <bits/types/time_t.h>
#include <ctime>
#include <iostream>
#include <string>

class TimeStamp {
public:
    TimeStamp() :
        micro_seconds_since_epoch_(0)
    {  }

    explicit TimeStamp(int64_t micro_seconds_since_epoch) :
        micro_seconds_since_epoch_(micro_seconds_since_epoch)
    {  }

    void swap(TimeStamp &that) {
        std::swap(micro_seconds_since_epoch_, that.micro_seconds_since_epoch_);
    }

    std::string to_string() const;
    std::string to_formatted_string(bool show_microseconds = true) const;

    bool valid() const {
        return micro_seconds_since_epoch_ > 0;
    }

    int64_t micro_seconds_since_epoch() const {
        return micro_seconds_since_epoch_;
    }

    time_t seconds_since_epoch() const {
        return static_cast<time_t>(micro_seconds_since_epoch_ / kMicroSecondsPerSecond);
    }


    static TimeStamp now();
    static TimeStamp invalid() {
        return TimeStamp();
    }
    static TimeStamp from_unix_time(time_t t) {
        return from_unix_time(t, 0);
    }
    static TimeStamp from_unix_time(time_t t, int micro_seconds) {
        return TimeStamp(static_cast<int64_t>(t) * kMicroSecondsPerSecond + micro_seconds);
    }

    static const int kMicroSecondsPerSecond = 1000 * 1000;
private:
    int64_t micro_seconds_since_epoch_;
};


inline bool operator<(TimeStamp lhs, TimeStamp rhs) {
    return lhs.micro_seconds_since_epoch() < rhs.micro_seconds_since_epoch();
}

inline bool operator==(TimeStamp lhs, TimeStamp rhs) {
    return lhs.micro_seconds_since_epoch() == rhs.micro_seconds_since_epoch();
}

//return seconds
inline double time_diff(TimeStamp high, TimeStamp low) {
    int64_t diff = high.micro_seconds_since_epoch() - low.micro_seconds_since_epoch();
    return static_cast<double>(diff) / TimeStamp::kMicroSecondsPerSecond;
}

inline TimeStamp add_time(TimeStamp timestamp, double seconds) {
    int64_t delta = static_cast<int64_t>(seconds * TimeStamp::kMicroSecondsPerSecond);
    return TimeStamp(timestamp.micro_seconds_since_epoch() + delta);
}


#endif
