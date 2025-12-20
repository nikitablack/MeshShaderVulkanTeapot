cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

add_library(utils INTERFACE)

add_library(libs::utils
    ALIAS
        utils
)

target_sources(utils
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/utils/try_expected.hpp
)

target_include_directories(utils
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
