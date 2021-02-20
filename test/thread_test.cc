/*
	* File     : thread_test.cc
	* Author   : *
	* Mail     : *
	* Creation : Wed 27 Jan 2021 11:21:51 AM CST
*/

#include <functional>
#include <iostream>
#include "../include/Thread.h"
#include "../include/ConCurrency.h"


void thread_func() {
    std::cout << "hello world" << std::endl;
}

class A {
public:
    void print() {
        std::cout << "this is A" << std::endl;
    }
};

Mutex mutex;
int n = 0;
void func() {
    for(int i = 0; i < 10000; ++i) {
        MutexLockGuard guard(mutex);
        n = n + 1;
    }
}


int main(int argc, char *argv[])
{
    Thread t1(thread_func);
    t1.start();

    sleep(1);


    Thread t2(func);
    Thread t3(func);
    Thread t4(func);
    Thread t5(func);
    t2.start();
    t3.start();
    t4.start();
    t5.start();

    sleep(2);
    std::cout << n << std::endl;


    return 0;
}
