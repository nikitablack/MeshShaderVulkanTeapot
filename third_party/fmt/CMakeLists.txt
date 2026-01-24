cmake_minimum_required(VERSION 3.25)

set(FMT_VERSION 12.1.0)
set(FMT_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/${FMT_VERSION}")

if(EXISTS "${FMT_SRC_DIR}/CMakeLists.txt")
    add_subdirectory("${FMT_SRC_DIR}")
else()
    message(STATUS "Downloading fmt ${FMT_VERSION}")

    FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG ${FMT_VERSION}
        SOURCE_DIR ${FMT_SRC_DIR}
    )

    FetchContent_MakeAvailable(fmt)
endif()
