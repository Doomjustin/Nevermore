include(AddFTXUI)

add_library(calc_console_static STATIC
    include/tui.h
    tui.cpp
)

target_include_directories(calc_console_static PUBLIC include)

target_precompile_headers(calc_console_static PUBLIC <string>)

target_link_libraries(calc_console_static PUBLIC
    calc_shared
    ftxui::screen
    ftxui::dom
    ftxui::component
)

include(BuildInfo)
sf_build_info(calc_console_static)

add_executable(calc_console bootstrap.cpp)
target_link_libraries(calc_console PRIVATE calc_console_static)
