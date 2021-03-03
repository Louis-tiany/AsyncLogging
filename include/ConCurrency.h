/*
    * File     : ConCurrency.h
    * Author   : *
    * Mail     : *
    * Creation : Wed 27 Jan 2021 02:42:04 PM CST
*/

#ifndef _CONCURRENCY_H
#define _CONCURRENCY_H
#include <pthread.h>
#include <errno.h>

extern int tid();

class Condition;

class Mutex {
public:
    Mutex(Mutex &) = delete;
    Mutex& operator=(Mutex &) = delete;

    Mutex() : 
        holder_(0)
    { pthread_mutex_init(&mutex_, NULL); }
    

    ~Mutex() {
        pthread_mutex_destroy(&mutex_);
    }
    void lock() {
        pthread_mutex_lock(&mutex_);

    }
    void unlock() {
        pthread_mutex_unlock(&mutex_);
    }

    pthread_mutex_t* get_mutex() {
        return &mutex_;
    }

private:
    friend class Condition;
    class UnassignGuard {
    public:
        explicit UnassignGuard(Mutex &owner) : 
            owner_(owner)
        { owner_.unassign_holder(); }
        ~UnassignGuard() {
            owner_.assign_holder();
        }
    private:
        Mutex &owner_;
    };

    void unassign_holder() {
        holder_ = 0;
    }

    void assign_holder() {
        holder_ = tid();
    }

private:
    pthread_mutex_t mutex_;
    pid_t holder_;
};




class Condition {
public:
    static const int64_t kNanoSecondsPerSeconds = 1000000000;
    explicit Condition(Mutex &mutex) : 
        mutex_(mutex)
    { pthread_cond_init(&cond_, NULL); }

    ~Condition() {
        pthread_cond_destroy(&cond_);
    }

    void wait() {
        Mutex::UnassignGuard guard(mutex_);
        pthread_cond_wait(&cond_, mutex_.get_mutex());
    }

    bool wait_for_seconds(double seconds) {
        struct timespec abstime;
        clock_gettime(CLOCK_REALTIME, &abstime);
        int64_t nanoseconds = static_cast<int64_t>(seconds * kNanoSecondsPerSeconds);

        abstime.tv_sec += static_cast<time_t>((abstime.tv_nsec + nanoseconds) / kNanoSecondsPerSeconds);
        abstime.tv_nsec = static_cast<long>((abstime.tv_nsec + nanoseconds) % kNanoSecondsPerSeconds);

        Mutex::UnassignGuard guard(mutex_);
        return ETIMEDOUT == pthread_cond_timedwait(&cond_, mutex_.get_mutex(), &abstime);
    }

    void notify() {
        pthread_cond_signal(&cond_);
    }

    void notify_all() {
        pthread_cond_broadcast(&cond_);
    }



private:
    Mutex &mutex_;
    pthread_cond_t cond_;
};


#endif
