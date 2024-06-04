include(FetchContent)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY    https://github.com/fmtlib/fmt.git # 指定git仓库地址
    GIT_TAG           10.2.0 # 指定版本
    SOURCE_DIR        ${PROJECT_SOURCE_DIR}/extern/fmt   # 源码下载到此目录下
)

FetchContent_MakeAvailable(fmt)
