add_library(nevermore_common INTERFACE)

target_include_directories(nevermore_common INTERFACE
    $<BUILD_INTERFACE: ${CMAKE_CURRENT_LIST_DIR}/include>
)

include(BuildInfo)
sf_build_info(nevermore_common)
