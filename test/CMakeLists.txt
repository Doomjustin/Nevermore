list(APPEND 
    CMAKE_MODULE_PATH 
        ${PROJECT_SOURCE_DIR}/cmake
        ${catch2_SOURCE_DIR}/extras
)
include(AddCatch2)
include(CTest)
include(Catch)