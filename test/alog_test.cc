/*
	* File     : alog_test.cc
	* Author   : *
	* Mail     : *
	* Creation : Thu 28 Jan 2021 05:41:36 PM CST
*/

#include <iostream>
#include "../AsyncLogging.h"



int main(int argc, char *argv[])
{
    AsyncLogging log("log.log", 10);
    log.start();
    sleep(4);
    char ss[] = "lkkkkkkkkkkkkkkkkk\n";
    for (int i = 0; i < 100000; ++i) {
        log.append(ss, strlen(ss));
    }

    return 0;
}
