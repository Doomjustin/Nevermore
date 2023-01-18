function(sf_add_daytime target source)
    add_executable(daytime-${target} ${source})
    
    set_target_properties(daytime-${target}
        PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/DayTime/
    )
    
    target_link_libraries(daytime-${target}
        PRIVATE
            fmt::fmt
            spdlog::spdlog
            Nevermore::Net
    )

endfunction()

sf_add_daytime(client-v4 client_v4.cpp)
sf_add_daytime(server-v4 server_v4.cpp)
sf_add_daytime(server-v6 server_v6.cpp)
sf_add_daytime(client-v6 client_v6.cpp)