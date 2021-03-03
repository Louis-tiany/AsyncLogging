/*
    * File     : LogStream.h
    * Author   : *
    * Mail     : *
    * Creation : Tue 26 Jan 2021 11:42:42 AM CST
*/

#ifndef _LOGSTREAM_H
#define _LOGSTREAM_H
#include <bits/c++config.h>
#include <iostream>
#include <string.h>
#include <string>



template<int SIZE>
class FixedBuffer {
public:

    FixedBuffer() : 
        cur_(data_) 
    { 
        set_cookie(cookie_start);
    }

    ~FixedBuffer() {
        set_cookie(cookie_end);
    }

    void append(const char *buf, std::size_t len) {
        if (static_cast<size_t>(avail()) > len) {
            memcpy(cur_, buf, len);
            cur_ += len;
        }
    }

    const char* data() const {
        return data_;
    }
    int length() const {
        return static_cast<int>(cur_ - data_);
    }
    char *current() {
        return cur_;
    }
    int avail() const {
        return static_cast<int>(end() - cur_);
    }
    void add(size_t len) {
        cur_ += len;
    }
    void thereset() {
        cur_ = data_;
        printf("bufffer reset called\n");
    }
    void bzero() {
        ::memset(data_, 0, sizeof(data_));
    }
    std::string to_string() const {
        return std::string(data_, length());
    }

    void set_cookie(void (*cookie)()) {
        cookie_ = cookie;
    }

private:
    const char* end() const {
        return data_ + sizeof(data_);
    }

    static void cookie_start() {

    }
    static void cookie_end() {

    }

private:
    void (*cookie_)();
    char data_[SIZE];
    char *cur_;
};


class LogStream {
public:
    static const int kSmallSize = 4000;
    static const int kLargeSize = 4000 * 1000;

    using Buffer = FixedBuffer<kSmallSize>;
    LogStream& operator<<(bool v){
        buffer_.append(v ? "1" : "0", 1);
        return *this;
    }

    LogStream& operator<<(short);
    LogStream& operator<<(unsigned short);
    LogStream& operator<<(int);
    LogStream& operator<<(unsigned int);
    LogStream& operator<<(long);
    LogStream& operator<<(unsigned long);
    LogStream& operator<<(long long);
    LogStream& operator<<(unsigned long long);

    LogStream& operator<<(const void *);
    LogStream& operator<<(float v) {
        *this << static_cast<double>(v);
        return *this;
    }
    LogStream& operator<<(double);
    LogStream& operator<<(char v) {
        buffer_.append(&v, 1);
        return *this;
    }
    
    LogStream& operator<<(const char *v) {
        if(v) {
            buffer_.append(v, strlen(v));
        }
        else {
            buffer_.append("(null)", 6);
        }
        return *this;
    }
    LogStream& operator<<(const unsigned char *str) {
        return operator<<(reinterpret_cast<const char *>(str));
    }
    LogStream& operator<<(std::string str) {
        buffer_.append(str.c_str(), str.size());
        return *this;
    }
    LogStream& operator<<(const Buffer &v) {
        *this << v.to_string();
        return *this;
    }


    void append(const char *data, int len) {
        buffer_.append(data, len);
    }
    const Buffer& buffer() const {
        return buffer_;
    }
    void reset_buffer() {
        buffer_.thereset();
    }


    LogStream() = default;
private:
    void static_check();
    template<class T>
    void format_interger(T);


private:
    Buffer buffer_;
    static const int kMaxNumericSize = 32;
};


class Fmt {
public: 
    template<class T>
    Fmt(const char *fmt, T val);

    const char *data() const {
        return buf_;
    }
    int length() const {
        return length_;
    }

private:
    char buf_[32];
    int length_;
};


inline LogStream& operator<<(LogStream &s, const Fmt &fmt) {
    s.append(fmt.data(), fmt.length());
    return s;
}

std::string formatSI(int64_t n);
std::string formatIEC(int64_t n);

#endif
