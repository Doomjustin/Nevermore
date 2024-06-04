add_library(nevermore_common OBJECT
    include/nevermore/Bits.h
    include/nevermore/EnumOperator.h
    include/nevermore/FunctorWrapper.h
    include/nevermore/Heap.h
    include/nevermore/Noncopyable.h
    include/nevermore/Singleton.h
    include/nevermore/StringUtils.h
    include/nevermore/ThreadPool.h
    include/nevermore/Timer.h
    ThreadPool.cpp
)

add_library(nevermore::common ALIAS nevermore_common)

target_include_directories(nevermore_common PUBLIC include)

include(BuildInfo)
sf_build_info(nevermore_common)
