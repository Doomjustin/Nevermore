add_library(nevermore_database
    include/nevermore/Database.h
    include/nevermore/Varint.h
    include/nevermore/WriteBatch.h
    Database.cpp
    WriteBatch.cpp
)
add_library(nevermore::database ALIAS nevermore_database)

target_include_directories(nevermore_database PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(nevermore_database PUBLIC nevermore::utility)