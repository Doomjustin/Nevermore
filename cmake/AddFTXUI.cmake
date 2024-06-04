include(FetchContent)

FetchContent_Declare(
    FTXUI
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
    GIT_TAG        v0.11
    SOURCE_DIR        ${PROJECT_SOURCE_DIR}/extern/FTXUI # 源码下载到此目录下
)

option(FTXUI_ENABLE_INSTALL "" OFF)
option(FTXUI_BUILD_EXAMPLES "" OFF)
option(FTXUI_BUILD_DOCS "" OFF)

FetchContent_MakeAvailable(FTXUI)
