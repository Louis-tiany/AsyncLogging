# AsyncLogging
多线程异步日志库

## Introduction
C++异步日志采用了日志缓冲区的方式提高效率写入压力大时缓冲区个数可自动扩展
通常情况工作时为双缓冲区
日志类启动时创建一个后台flush线程用于写入log文件

## Build and Run
### Build
```
git clone https://github.com/Louis-tiany/AsyncLogging.git
cd AsyncLogging
sh ./autobuild.sh
```

### Test
```
cd build/test
time ./alog.out 
```

## Performance Test
- CPU : Intel 9700K
- Memory : 16GB
- OS : Ubuntu 20.04
- complier : gcc

单线程写入100W条日志:
四业务线程分别写入100W条日志:

## Reference 
[陈硕muduo库](https://github.com/chenshuo/muduo/)
