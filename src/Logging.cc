/*
    * File     : Logging.cc
    * Author   : *
    * Mail     : *
    * Creation : Sun 31 Jan 2021 08:54:50 AM CST
*/

#include <bits/types/time_t.h>
#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include "../include/Logging.h"

__thread char t_errorbuf[512];
__thread char t_time[64];
__thread time_t t_last_second;


Logger::Level g_level = Logger::Level::TRACE;


const std::unordered_map<Logger::Level, const char *> log_level_table = {
    {Logger::Level::TRACE, "[TRACE] "},
    {Logger::Level::DEBUG, "[DEBUG] "},
    {Logger::Level::INFO, "[INFO]  "},
    {Logger::Level::WARN, "[WARN]  "},
    {Logger::Level::FATAL, "[FATAL] "},
};



class T {
public:
    T(const char *str, unsigned int len) :
        str_(str),
        len_(len)
    {  }
    const char *str_;
    const unsigned int len_;
};

inline LogStream& operator<<(LogStream &s, T v) {
    s.append(v.str_, v.len_);
    return s;
}

inline LogStream& operator<<(LogStream &s, const Logger::SourceFile &v) {
    s.append(v.data_, v.size_);
    return s;
}

void default_output(const char *msg, int len) {
    fwrite(msg, 1, len, stdout);
}

void default_flush() {
    fflush(stdout);
}

Logger::OutputFunc g_output = default_output;
Logger::FlushFunc g_flush = default_flush;


Logger::Impl::Impl(Level level, int old_errno, const SourceFile& file, int line) :
    time_(TimeStamp::now()),
    stream_(),
    level_(level),
    line_(line),
    basename_(file)
{ 
    format_time();
    stream_ << T(log_level_table.at(level), 6);
}

void Logger::Impl::Impl::format_time() {
    int64_t micro_seconds_since_epoch = time_.micro_seconds_since_epoch();
    time_t seconds = static_cast<time_t>(micro_seconds_since_epoch / TimeStamp::kMicroSecondsPerSecond);
    int micro_seconds = static_cast<int>(micro_seconds_since_epoch % TimeStamp::kMicroSecondsPerSecond);
    if (seconds != t_last_second) {
        t_last_second = seconds;
        struct tm t;

        gmtime_r(&seconds, &t);
        
        int len = snprintf(t_time, sizeof(t_time), "%4d%02d%02d %02d:%02d:%02d", 
                           t.tm_year + 1900, t.tm_mon, t.tm_mday, t.tm_hour,
                           t.tm_min, t.tm_sec);
    }

    Fmt us(".%06d ", micro_seconds);
    stream_ << T(t_time, 17) << T(us.data(), 8);
}


void Logger::Impl::Impl::finsh() {
    stream_ << " - " << basename_ << ':' << line_ << "\n";
}


Logger::Logger(SourceFile file, int line) : 
    impl_(Logger::Level::INFO, 0, file, line)
{  }

Logger::Logger(SourceFile file, int line, Level level) : 
    impl_(level, 0, file, line)
{  }

Logger::Logger(SourceFile file, int line, Level level, const char *func) : 
    impl_(level, 0, file, line)
{  
    impl_.stream_ << func << ' ';
}


Logger::Logger(SourceFile file, int line, bool to_abort) :
    impl_(to_abort ? Logger::Level::FATAL : Logger::Level::ERROR, errno, file, line)
{  }

Logger::~Logger() {
    impl_.finsh();
    const LogStream::Buffer &buf(stream().buffer());
    g_output(buf.data(), buf.length());
    if (impl_.level_ == Logger::Level::FATAL) {
        g_flush();
        abort();
    }
}

void Logger::set_log_level(Logger::Level level) {
    g_level = level;
}

void Logger::set_output(OutputFunc out) {
    g_output = out;
}
void Logger::set_flush(FlushFunc func) {
    g_flush = func;
}
