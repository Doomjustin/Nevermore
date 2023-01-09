function(sf_add_core_test target source)
    add_executable(${target} ${source})
    
    set_target_properties(${target}
        PROPERTIES 
            RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/Core/
    )

    target_link_libraries(${target} 
        PRIVATE 
            Catch2::Catch2WithMain 
            Nevermore::Core)
    catch_discover_tests(${target})
endfunction()

sf_add_core_test(test-utility test_utility.cpp)