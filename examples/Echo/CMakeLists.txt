function(sf_add_echo target source)
    add_executable(echo-${target} ${source})
    
    set_target_properties(echo-${target}
        PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/Echo/
    )
    
    target_link_libraries(echo-${target}
        PRIVATE
            fmt::fmt
            spdlog::spdlog
            Nevermore::Net
    )
endfunction()

sf_add_echo(server-v4 server_v4.cpp)
sf_add_echo(client-v4 client_v4.cpp)