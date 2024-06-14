add_library(nevermore_posix INTERFACE
    include/nevermore/posix/Memory.h
)
add_library(nevermore::posix ALIAS nevermore_posix)

target_include_directories(nevermore_posix INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include/)