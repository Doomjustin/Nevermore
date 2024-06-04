add_executable(test-timer test_timer.cpp)
target_link_libraries(test-timer PRIVATE nevermore::common)

add_executable(test-thread-pool test_thread_pool.cpp)
target_link_libraries(test-thread-pool PRIVATE nevermore::common)