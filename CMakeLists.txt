cmake_minimum_required(VERSION 2.8)

project(AsyncLog)
set(CMAKE_CXX_FLAGS   "-std=c++11")             # c++11
set(CMAKE_CXX_FLAGS   "-g")                     # 调试信息
set(CMAKE_CXX_FLAGS   "-Wall")                  # 开启所有警告
set(CMAKE_BUILD_TYPE RELEASE)

include_directories(./include)
aux_source_directory(./src DIR_SRCS)

SET(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/lib")

add_library(alog STATIC ${DIR_SRCS})
link_directories(${PROJECT_SOURCE_DIR}/lib)

add_executable(test.out test/test.cc)

add_executable(alog.out test/alog_test.cc)
target_link_libraries(alog.out libalog.a pthread)

add_executable(logger_test.out test/loger_test.cc)
target_link_libraries(logger_test.out libalog.a pthread)

add_executable(thread_test.out test/thread_test.cc)
target_link_libraries(thread_test.out pthread libalog.a)

