/*
	* File     : Logging.h
	* Author   : *
	* Mail     : *
	* Creation : Sun 31 Jan 2021 08:54:47 AM CST
*/

#ifndef _LOGGING_H
#define _LOGGING_H

#include <ctime>
#include <iostream>
#include <string.h>

#include "LogStream.h"
#include "TimeStamp.h"

class Logger {
public:
    enum class Level {
        TRACE,
        DEBUG, 
        INFO, 
        WARN,
        ERROR,
        FATAL,
    };


    class SourceFile {
    public:
        template<int N>
        SourceFile(const char (&arr)[N]) :
            data_(arr),
            size_(N - 1)
        { 
            const char *slash = strchr(data_, '/');
            if (slash) {
                data_ = slash + 1;
                size_ -= static_cast<int>(data_ - arr);
            }
        }

        explicit SourceFile(const char *filename) {
            const char *slash = strchr(data_, '/');
            if (slash) {
                data_ = slash + 1;
            }
            size_ = static_cast<int>(strlen(data_));
        }

        const char *data_;
        int size_;
    };

    Logger(SourceFile file, int line);
    Logger(SourceFile file, int line, Level level);
    Logger(SourceFile file, int line, Level level, const char *func);
    Logger(SourceFile file, int line, bool to_abort);

    ~Logger();

    LogStream& stream() {
        return impl_.stream_;
    }

    static Level log_level();
    static void set_log_level(Level level);



    typedef void(*OutputFunc)(const char *msg, int len);
    typedef void(*FlushFunc)();
    
    static void set_output(OutputFunc);
    static void set_flush(FlushFunc);
    //static void set_timezone();

private:
    class Impl {
    public:
        using Level = Logger::Level;

        Impl(Level level, int old_errno, const SourceFile& file, int line);
        void format_time();
        void finsh();

        TimeStamp time_;
        LogStream stream_;
        Level level_;
        int line_;
        SourceFile basename_;
    };
private:
    Impl impl_;
};



extern Logger::Level g_level;
inline Logger::Level Logger::log_level() {
    return g_level;
}


#define LOG_TRACE if(Logger::log_level() <= Logger::Level::TRACE)\
                    Logger(__FILE__, __LINE__, Logger::Level::TRACE, __func__).stream()

#define LOG_DEBUG if(Logger::log_level() <= Logger::Level::DEBUG)\
                    Logger(__FILE__, __LINE__, Logger::Level::TRACE, __func__).stream()

#define LOG_INFO if(Logger::log_level() <= Logger::Level::INFO)\
                    Logger(__FILE__, __LINE__).stream()

#define LOG_WARN Logger(__FILE__, __LINE__, Logger::Level::WARN).stream()
#define LOG_ERROR Logger(__FILE__, __LINE__, Logger::Level::ERROR).stream()
#define LOG_FATAL Logger(__FILE__, __LINE__, Logger::Level::FATAL).stream()


#endif
