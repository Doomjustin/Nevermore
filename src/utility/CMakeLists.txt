include(AddSpdlog)

add_library(nevermore_utility OBJECT
    include/nevermore/Bits.h
    include/nevermore/EnumOperator.h
    include/nevermore/Heap.h
    include/nevermore/Log.h
    include/nevermore/Memory.h
    include/nevermore/Noncopyable.h
    include/nevermore/Singleton.h
    include/nevermore/StringUtils.h
    include/nevermore/ThreadPool.h
    include/nevermore/Timer.h
    Log.cpp
    ThreadPool.cpp
)
add_library(nevermore::utility ALIAS nevermore_utility)

target_include_directories(nevermore_utility PUBLIC include)

target_link_libraries(nevermore_utility PRIVATE spdlog::spdlog)

include(BuildInfo)
sf_build_info(nevermore_utility)
