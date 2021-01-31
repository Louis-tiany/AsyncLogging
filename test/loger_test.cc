/*
	* File     : loger_test.cc
	* Author   : *
	* Mail     : *
	* Creation : Sun 31 Jan 2021 06:58:08 PM CST
*/

#include <iostream>
#include "../Logging.h"


int main(int argc, char *argv[])
{
    for(int i = 0; i < 10; ++i) {
        LOG_TRACE << "logger lib test";
    }
    return 0;
}
