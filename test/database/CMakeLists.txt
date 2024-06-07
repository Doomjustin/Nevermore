macro(sf_add_datase_test target)
    target_link_libraries(${target} PRIVATE 
        Catch2::Catch2WithMain
        nevermore::database
    )
    catch_discover_tests(${target})
endmacro()

add_executable(test-status test_status.cpp)
sf_add_datase_test(test-status)