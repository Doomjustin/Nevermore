list(APPEND 
    CMAKE_MODULE_PATH 
        ${PROJECT_SOURCE_DIR}/cmake
        ${catch2_SOURCE_DIR}/extras
)
include(AddCatch2)
include(CTest)
include(Catch)

add_subdirectory(Core)

add_executable(sf-test-unit test_unit.cpp)
target_include_directories(sf-test-unit PRIVATE ${PROJECT_SOURCE_DIR}/include/)