add_library(nevermore_posix STATIC
    include/nevermore/posix/File.h
    include/nevermore/posix/Memory.h
    include/nevermore/posix/Writable.h
    File.cpp
)
add_library(nevermore::posix ALIAS nevermore_posix)

target_include_directories(nevermore_posix PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include/)