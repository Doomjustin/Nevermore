include(FetchContent)

FetchContent_Declare(
    pybind11
    GIT_REPOSITORY    https://github.com/pybind/pybind11.git  # 指定git仓库地址
    GIT_TAG           v2.12.0 # 指定版本
    SOURCE_DIR        ${PROJECT_SOURCE_DIR}/extern/pybind11   # 源码下载到此目录下
)

FetchContent_MakeAvailable(pybind11)