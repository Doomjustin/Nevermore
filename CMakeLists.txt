cmake_minimum_required(VERSION 3.28)
project(Nevermore VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(NoInSourceBuild)

add_subdirectory(src bin)
add_subdirectory(test)

include(Doxygen)
sf_doxygen(src docs)