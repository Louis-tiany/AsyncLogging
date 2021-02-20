/*
    * File     : AsyncLogging.cc
    * Author   : *
    * Mail     : *
    * Creation : Mon 25 Jan 2021 11:36:00 AM CST
*/

#include <algorithm>
#include <iostream>
#include "../include/AsyncLogging.h"


AsyncLogging::AsyncLogging(std::string basename, off_t roll_size, int flush_interval) : 
    flush_interval_(flush_interval),
    is_running_(false),
    basename_(basename),
    roll_size_(roll_size),
    thread_(std::bind(&AsyncLogging::thread_func, this)),
    mutex_(),
    cond_(mutex_),
    current_buffer_(new Buffer()),
    next_buffer_(new Buffer()),
    buffers_()
{
    current_buffer_->bzero();
    next_buffer_->bzero();
    buffers_.reserve(16);
}

void AsyncLogging::append(const char *logline, int len) {
    MutexLockGuard guard(mutex_);
    ddd(current_buffer_->avail());
    if (current_buffer_->avail() > len) {
        current_buffer_->append(logline, len);
        ddd("append avail");
    }
    else {
        buffers_.push_back(std::move(current_buffer_));
        ddd("append not avail");
        if (next_buffer_) {
            current_buffer_ = std::move(next_buffer_);
        }
        else {
            current_buffer_.reset(new Buffer());
        }
        current_buffer_->append(logline, len);
        ddd("append current_buffer");
        cond_.notify();
        ddd("notify");
    }
}


void AsyncLogging::thread_func() {
    LogFile output(basename_, roll_size_, false);
    BufferPtr new_buffer1(new Buffer);
    BufferPtr new_buffer2(new Buffer);

    ddd("new buf2 ptr");
    ddd(new_buffer2.get());
    ddd(new_buffer1.get());

    new_buffer1->bzero();
    new_buffer2->bzero();

    BufferVector buffers2write;
    buffers2write.reserve(16);
    while (is_running_) {
        {
            MutexLockGuard guard(mutex_);
            if (buffers_.empty()) {
                cond_.wait_for_seconds(flush_interval_);
            }
            buffers_.push_back(std::move(current_buffer_));
            current_buffer_ = std::move(new_buffer1);
            buffers2write.swap(buffers_);
            if (!next_buffer_) {
                next_buffer_ = std::move(new_buffer2);
            }
        }

        if (buffers2write.size() > 25) {
            char buf[256];
            buffers2write.erase(buffers2write.begin() + 2, buffers2write.end());
            ddd("erase buffers2write");
        }

        ddd("buffers2write size");
        ddd(buffers2write.size());
        for(const auto &buffer : buffers2write) {
            output.append(buffer->data(), buffer->length());
            //ddd("logfile output append");
        }
        
        if (buffers2write.size() > 2) {
            buffers2write.resize(2);
        }

        ddd("new buf1 ptr");
        ddd(new_buffer1.get());
        ddd(new_buffer2.get());
        if (!new_buffer1) {
            new_buffer1 = std::move(buffers2write.back());
            buffers2write.pop_back();
            new_buffer1->thereset();
            ddd("new buffer1 reset");
            ddd(new_buffer1.get());
        }
        if (!new_buffer2) {
            new_buffer2 = std::move(buffers2write.back());
            buffers2write.pop_back();
            new_buffer2->thereset();
            ddd("new buffer2 reset");
            ddd(new_buffer2.get());
        }
        buffers2write.clear();
        output.flush();
    }
    output.flush();
    ddd("after one loop");
}
