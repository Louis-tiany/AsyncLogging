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

add_subdirectory(test)
