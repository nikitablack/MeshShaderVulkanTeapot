cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

add_executable(teapot)

target_sources(teapot
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

set_target_properties(teapot
    PROPERTIES
        CXX_STANDARD 23
        CXX_SATANDARD_REQUIRED YES
        CXX_EXTENSION NO
)

target_link_libraries(teapot
    PRIVATE
        fmt::fmt
        libs::utils
        libs::warnings_as_errors
        libs::window
)
