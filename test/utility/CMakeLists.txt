add_executable(test-timer test_timer.cpp)
target_link_libraries(test-timer PRIVATE nevermore::utility)

add_executable(test-thread-pool test_thread_pool.cpp)
target_link_libraries(test-thread-pool PRIVATE nevermore::utility)

add_executable(test-delegate test_delegate.cpp)
target_link_libraries(test-delegate PRIVATE nevermore::utility)

add_executable(test-memory test_memory.cpp)
target_link_libraries(test-memory PRIVATE nevermore::utility)

add_executable(test-log test_log.cpp)
target_link_libraries(test-log PRIVATE nevermore::utility)
