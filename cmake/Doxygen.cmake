find_package(Doxygen REQUIRED)

include(FetchContent)
FetchContent_Declare(doxygen-awesome-css
    GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css.git
    GIT_TAG v1.6.0
    SOURCE_DIR        ${PROJECT_SOURCE_DIR}/extern/doxygen-awesome-css # 源码下载到此目录下
)

FetchContent_MakeAvailable(doxygen-awesome-css)

function(doxygen target input)
    set(NAME "doxygen-${target}")
    set(DOXYGEN_HTML_OUTPUT
        ${PROJECT_BINARY_DIR}/${NAME})
    set(DOXYGEN_GENERATE_HTML         YES)
    set(DOXYGEN_GENERATE_TREEVIEW     YES)
    set(DOXYGEN_HAVE_DOT              YES)
    set(DOXYGEN_DOT_IMAGE_FORMAT      svg)
    set(DOXYGEN_DOT_TRANSPARENT       YES)
    set(DOXYGEN_HTML_EXTRA_STYLESHEET
        ${doxygen-awesome-css_SOURCE_DIR}/doxygen-awesome.css)
    doxygen_add_docs(${NAME}
        ${PROJECT_SOURCE_DIR}/${input}
        COMMENT "Generate HTML documentation"
    )
endfunction()
