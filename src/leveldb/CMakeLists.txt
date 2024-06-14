add_library(nevermore_leveldb OBJECT
    include/nevermore/Coding.h
    include/nevermore/Database.h
    include/nevermore/Format.h
    include/nevermore/InternalKey.h
    include/nevermore/Logging.h
    include/nevermore/Option.h
    include/nevermore/Status.h
    include/nevermore/WriteBatch.h
    Coding.cpp
    Database.cpp
    InternalKey.cpp
    Logging.cpp
    Status.cpp
    WriteBatch.cpp
)

add_library(nevermore::leveldb ALIAS nevermore_leveldb)

target_include_directories(nevermore_leveldb PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(nevermore_leveldb PUBLIC nevermore::utility)