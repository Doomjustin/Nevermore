add_library(nevermore_common OBJECT
    include/Bits.h
    include/EnumOperator.h
    include/FunctorWrapper.h
    include/Heap.h
    include/Noncopyable.h
    include/Singleton.h
    include/StringUtils.h
    include/ThreadPool.h
    include/Timer.h
    ThreadPool.cpp
)

target_include_directories(nevermore_common PUBLIC
    $<BUILD_INTERFACE: ${CMAKE_CURRENT_LIST_DIR}/include>
)

include(BuildInfo)
sf_build_info(nevermore_common)
