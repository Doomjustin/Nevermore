macro(sf_add_db_test target)
    target_link_libraries(${target} PRIVATE 
        Catch2::Catch2WithMain
        nevermore::database
    )

    target_include_directories(${target} PRIVATE ${PROJECT_SOURCE_DIR}/src/database)

    catch_discover_tests(${target})
endmacro()


add_executable(db-test test.cpp)

add_executable(test-varint test_varint.cpp)
sf_add_db_test(test-varint)