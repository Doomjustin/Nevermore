if (APPLE)
    set(SF_IS_WIN32 false)
    set(SF_IS_UNIX false)
    set(SF_IS_APPLE true)
    set(SF_PLATFORM SF_APPLE)
elseif(UNIX)
    set(SF_IS_WIN32 false)
    set(SF_IS_UNIX true)
    set(SF_IS_APPLE false)
    set(SF_PLATFORM SF_UNIX)
    else()
    set(SF_IS_WIN32 true)
    set(SF_IS_UNIX false)
    set(SF_IS_APPLE false)
    set(SF_PLATFORM SF_WINDOWS)
endif()

set(SF_PROJECT_DIR ${PROJECT_SOURCE_DIR})
set(SF_INTERNAL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/Nevermore)
set(SF_PUBLIC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

configure_file(
    Config.h.in
    ${PROJECT_SOURCE_DIR}/include/Nevermore/Config.h
)

add_library(NevermoreCore
    STATIC
        ${SF_INTERNAL_INCLUDE_DIR}/Config.h
        ${SF_INTERNAL_INCLUDE_DIR}/Utility.h
        ${SF_INTERNAL_INCLUDE_DIR}/FunctionWrapper.h
        ${SF_INTERNAL_INCLUDE_DIR}/ThreadPool.h
        ThreadPool.cpp
)

add_library(Nevermore::Core ALIAS NevermoreCore)

target_include_directories(NevermoreCore PRIVATE ${SF_INTERNAL_INCLUDE_DIR})
target_include_directories(NevermoreCore PUBLIC ${SF_PUBLIC_INCLUDE_DIR})

target_link_libraries(NevermoreCore
    PRIVATE
        fmt::fmt
        spdlog::spdlog
)