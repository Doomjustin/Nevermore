add_executable(test-spdlogger test_spdlogger.cpp)
target_link_libraries(test-spdlogger PRIVATE nevermore::log)
target_compile_definitions(test-spdlogger PRIVATE NEVERMORE_DEBUG)