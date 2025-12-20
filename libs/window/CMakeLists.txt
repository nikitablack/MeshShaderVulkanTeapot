cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

add_library(window STATIC)

add_library(libs::window
    ALIAS
        window
)

target_sources(window 
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/window/Window.hpp

        ${CMAKE_CURRENT_SOURCE_DIR}/src/window/Window.cpp
)

target_include_directories(window
    PUBLIC
        include
)

set_target_properties(window PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_link_libraries(window
    PUBLIC
        glfw
    PRIVATE
        libs::warnings_as_errors
)