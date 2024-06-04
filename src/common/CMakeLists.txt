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

target_include_directories(nevermore_common PUBLIC
    $<BUILD_INTERFACE: ${CMAKE_CURRENT_LIST_DIR}/include>
)

include(BuildInfo)
sf_build_info(nevermore_common)
