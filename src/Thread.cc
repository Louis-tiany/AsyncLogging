/*
	* File     : Thread.cc
	* Author   : *
	* Mail     : *
	* Creation : Tue 26 Jan 2021 10:29:33 AM CST
*/

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "../include/Thread.h"
#include <sys/syscall.h>

__thread int t_cached_tid = 0;
__thread char t_tid_string[32];
__thread int t_tid_string_length = 6;
__thread const char *t_thread_name = "unknown";

void cacheTid() {
    if (t_cached_tid== 0) {
        t_cached_tid= gettid();
        t_tid_string_length = snprintf(t_tid_string, sizeof t_tid_string, "%5d ", t_cached_tid);
    }
}


int tid(){
    if(t_cached_tid == 0) {
        cacheTid();
    }
    return t_cached_tid;
}



pid_t get_tid() {
    return static_cast<pid_t>(::syscall(SYS_gettid));
}

struct ThreadData {
    using ThreadFunc = Thread::ThreadFunc;
    ThreadFunc func_;
    pid_t *tid_;
    ThreadData(ThreadFunc func, pid_t *tid) :
        func_(std::move(func)),
        tid_(tid)
    {  }

    void run_in_thread() {
        *tid_ = tid();
        //tid_ = nullptr;
        func_();
    }
};


void *start_thread(void *obj) {
    ThreadData *data = static_cast<ThreadData *>(obj);
    data->run_in_thread();
    delete data;
    return nullptr;
}

Thread::Thread(ThreadFunc func) :
    started_(false),
    joined_(false),
    pthread_id_(0),
    tid_(0),
    func_(std::move(func))
{  }


Thread::~Thread() {
    if (started_ && !joined_) {
        pthread_detach(pthread_id_);
    }
}

void Thread::start() {
    started_ = true;
    ThreadData *data = new ThreadData(func_, &tid_);
    if ( pthread_create(&pthread_id_, NULL, &start_thread, data) ) {
        started_ = false;
        delete data;
        abort();
    }
}

int Thread::join() {
    joined_ = true;
   return pthread_join(pthread_id_, nullptr);
}
