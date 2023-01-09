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
set(SF_INTERNAL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(SF_PUBLIC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(SF_ENABLE_EXCEPTION SF_ENABLE_EXCEPTION)

configure_file(
    Config.h.in
    ${PROJECT_SOURCE_DIR}/include/Nevermore/Config.h
)