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

function(sf_add_core_test_without_catch2 target source)
    add_executable(${target} ${source})
        
    set_target_properties(${target}
        PROPERTIES 
            RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/Core/
    )

    target_link_libraries(${target} 
        PRIVATE 
            Nevermore::Core
    )
endfunction()


sf_add_core_test(test-utility test_utility.cpp)


sf_add_core_test_without_catch2(test-threadpool test_threadpool.cpp)