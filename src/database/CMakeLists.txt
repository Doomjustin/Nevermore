add_library(nevermore_database OBJECT
    include/nevermore/Database.h
    include/nevermore/Status.h
    Database.cpp
    Status.cpp
)

add_library(nevermore::database ALIAS nevermore_database)

target_include_directories(nevermore_database PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)