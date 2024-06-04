enable_testing()

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY    https://github.com/catchorg/Catch2.git  # 指定git仓库地址
    GIT_TAG           v3.6.0 # 指定版本
    SOURCE_DIR        ${PROJECT_SOURCE_DIR}/extern/Catch2   # 源码下载到此目录下
)

option(CATCH_INSTALL_DOCS "Install Catch2's documentation" OFF)

FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

macro(sf_add_test target)
    target_link_libraries(${target} PRIVATE Catch2::Catch2WithMain)
    catch_discover_tests(${target})
endmacro()
