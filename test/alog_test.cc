/*
	* File     : alog_test.cc
	* Author   : *
	* Mail     : *
	* Creation : Thu 28 Jan 2021 05:41:36 PM CST
*/

#include <iostream>
#include "../include/AsyncLogging.h"
#include "../include/Logging.h"

AsyncLogging *glog = nullptr;

void async(const char* msg, int len) {
    glog->append(msg, len);
}



int main(int argc, char *argv[])
{
    Logger::set_output(async);
    Logger::set_log_level(Logger::Level::TRACE);
    AsyncLogging log("log.log", 10);
    glog = &log;
    log.start();
    
    sleep(10);
    for (int i = 0; i < 10000000; ++i) {
        LOG_TRACE << "logger lib test";
    }

    for (int i = 0; i < 10000000; ++i) {
        LOG_INFO << "log info test";
    }
    LOG_FATAL << "fatal error";

    return 0;
}
