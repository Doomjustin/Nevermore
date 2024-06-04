include(AddSpdlog)

add_library(nevermore_log OBJECT
    include/nevermore/Log.h
    include/nevermore/Logger.h
    include/nevermore/LogLevel.h
    Log.cpp
    Logger.cpp
    SpdLogger.h
    SpdLogger.cpp
)

add_library(nevermore::log ALIAS nevermore_log)

target_include_directories(nevermore_log PUBLIC include)

target_link_libraries(nevermore_log PRIVATE spdlog::spdlog)

target_compile_definitions(nevermore_log PUBLIC NEVERMOR_DEBUG)