/*
	* File     : Thread.h
	* Author   : *
	* Mail     : *
	* Creation : Tue 26 Jan 2021 10:29:31 AM CST
*/

#ifndef _THREAD_H
#define _THREAD_H

#include <unistd.h>
#include <threads.h>
#include <functional>
#include "ConCurrency.h"

extern __thread int t_cached_tid;
extern __thread char t_tid_string[32];
extern __thread int t_tid_string_length;
extern __thread const char *t_thread_name;
void  cacheTid();

int tid();

class Thread {
public:
    using ThreadFunc = std::function<void()>;
    explicit Thread(ThreadFunc func);
    ~Thread();

    void start();
    int join();
    bool started() const {
        return started_;
    }

private:
    bool started_;
    bool joined_;
    pthread_t pthread_id_;
    pid_t tid_;
    ThreadFunc func_;

};


class MutexLockGuard {
public:
    explicit MutexLockGuard(Mutex &mutex) : 
        mutex_(mutex)
    { mutex_.lock(); }

    ~MutexLockGuard() {
        mutex_.unlock();
    }

private:
    Mutex &mutex_;
};


#endif
