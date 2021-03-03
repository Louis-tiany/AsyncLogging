add_executable(test.out test.cc)

add_executable(alog.out alog_test.cc)
target_link_libraries(alog.out alog pthread)

add_executable(logger_test.out loger_test.cc)
target_link_libraries(logger_test.out alog pthread)

add_executable(thread_test.out thread_test.cc)
target_link_libraries(thread_test.out pthread alog)

