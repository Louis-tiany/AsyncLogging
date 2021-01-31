/*
	* File     : AsyncLogging.h
	* Author   : *
	* Mail     : *
	* Creation : Mon 25 Jan 2021 11:35:56 AM CST
*/

#ifndef _ASYNCLOGGING_H
#define _ASYNCLOGGING_H
#include <iostream>
#include <atomic>
#include <memory>
#include <vector>
#include <mutex>
#include <condition_variable>
#include "LogStream.h"
#include "Thread.h"
#include "LogFile.h"
class AsyncLogging {
public:
    using Buffer = FixedBuffer<LogStream::kLargeSize>;
    using BufferVector = std::vector<std::unique_ptr<Buffer>>;
    using BufferPtr = BufferVector::value_type;
    AsyncLogging(std::string basename, off_t roll_size, int flush_interval = 3);
    ~AsyncLogging() {
        if (is_running_) {
            stop();
        }
    }

    void append(const char *logline, int len);
    void start() {
        is_running_ = true;
        thread_.start();
    }

    void stop() {
        is_running_ = false;
        cond_.notify();
        thread_.join();
    }


private:
    void thread_func();

private:
    const int flush_interval_;
    std::atomic<bool> is_running_;
    const std::string basename_;
    const off_t roll_size_;
    Thread thread_;
    Mutex mutex_;
    Condition cond_;
    BufferPtr current_buffer_;
    BufferPtr next_buffer_;
    BufferVector buffers_;
};



#endif
