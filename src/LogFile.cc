/*
    * File     : LogFile.cc
    * Author   : *
    * Mail     : *
    * Creation : Wed 27 Jan 2021 09:32:19 PM CST
*/

#include <iostream>
#include "../include/ConCurrency.h"
#include "../include/Thread.h"
#include "../include/LogFile.h"
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

AppendFile::AppendFile(const std::string filename) :
    fp_(::fopen(filename.c_str(), "ae")),
    written_bytes_(0)
{ 
    ::setbuffer(fp_, buffer_, sizeof(buffer_));
}

AppendFile::~AppendFile() {
    ::fclose(fp_);
}

void AppendFile::append(const char *logline, std::size_t len) {
    size_t n = write(logline, len);
    size_t remain = len - n;
    while (remain > 0) {
        size_t x = write(logline + n, remain);
        if (x == 0) {
            int err = ferror(fp_);
            if (err) {
                fprintf(stderr, "Appendfile error %s\n", strerror(err));
            }
            break;
        }
        n += x;
        remain = len - n;
    }
    written_bytes_ += len;
}

void AppendFile::flush() {
    ::fflush(fp_);
}

size_t AppendFile::write(const char *logline, size_t len) {
    return ::fwrite_unlocked(logline, 1, len, fp_);
}



LogFile::LogFile(const std::string &basename, off_t roll_size, bool thread_safe, int flush_interval, int check_everyn) : 
    basename_(basename),
    roll_size_(roll_size),
    flush_interval_(flush_interval),
    check_everyn_(check_everyn),
    count_(0),
    mutex_(thread_safe ? new Mutex : nullptr),
    start_of_peroid_(0),
    last_roll_(0),
    last_flush_(0)
{ roll_file(); }



void LogFile::append(const char *logline, int len) {
    if (mutex_) {
        MutexLockGuard guard(*mutex_);
        append_unlock(logline, len);
    }
    else {
        append_unlock(logline, len);
    }
}

void LogFile::flush() {
    ddd("begin log file flush");
    if (mutex_) {
        MutexLockGuard guard(*mutex_);
        ddd("log file flush");
        file_->flush();
    }
    else {
        ddd("no mutex flush")
        file_->flush();
    }
}

void LogFile::append_unlock(const char *logline, int len) {
    file_->append(logline, len);
    if (file_->written_bytes()) {
        roll_file();
    }
    else {
        ++count_;
        if (count_ >= check_everyn_) {
            count_ = 0;
            time_t now = ::time(NULL);
            time_t this_period_ = now / kRollPerSeconds * kRollPerSeconds;
            if (this_period_ != start_of_peroid_) {
                roll_file();
            }
            else if (now - last_roll_ > flush_interval_) {
                last_flush_ = now;
                file_->flush();
            }
        }
    }
}

bool LogFile::roll_file() {
    time_t now = 0;
    std::string filename = get_filename(basename_, &now);

    time_t start = now / kRollPerSeconds * kRollPerSeconds;
    if (now > last_roll_) {
        last_roll_ = now;
        last_flush_ = now;
        start_of_peroid_ = start;
        file_.reset(new AppendFile(filename));
        return true;
    }
    return false;
}


std::string hostname();
pid_t pid();
std::string LogFile::get_filename(const std::string &basename, time_t *now) {
    std::string name;
    name.reserve(basename.size() + 64);

    name = basename;
    char timebuf[32];
    struct tm t;
    *now = ::time(NULL);
    gmtime_r(now, &t);
    strftime(timebuf, sizeof timebuf, ".%Y%m%d-%H%M%S.", &t);
    name += timebuf;

    name += hostname();
    char pidbuf[32];
    snprintf(pidbuf, sizeof(pidbuf), "%d", pid());
    name += pidbuf;
    name += ".log";
    return name;
}

std::string hostname() {
    char buf[256];
    if (::gethostname(buf, sizeof buf) == 0) {
        buf[sizeof(buf)- 1] = '\0';
        return buf;
    }
    else {
        return "unknownhost";
    }
}

pid_t pid() {
    return ::getpid();
}

