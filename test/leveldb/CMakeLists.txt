macro(sf_add_db_test target)
    target_link_libraries(${target} PRIVATE 
        Catch2::Catch2WithMain
        nevermore::leveldb
    )
    catch_discover_tests(${target})
endmacro()

add_executable(test-status test_status.cpp)
sf_add_db_test(test-status)

add_executable(test-coding test_coding.cpp)
sf_add_db_test(test-coding)
