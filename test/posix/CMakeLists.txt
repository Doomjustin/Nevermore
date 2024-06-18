macro(sf_add_posix_test target)
    target_link_libraries(${target} PRIVATE 
        Catch2::Catch2WithMain
        nevermore::posix
    )
    catch_discover_tests(${target})
endmacro()

add_executable(test-memory test_memory.cpp)
sf_add_posix_test(test-memory)

add_executable(test-file test_file.cpp)
target_link_libraries(test-file PRIVATE nevermore::posix)