cmake_minimum_required(VERSION 3.25)

set(CMAKERC_VERSION 952ffdd)
set(CMAKERC_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/${CMAKERC_VERSION}")

if(EXISTS "${CMAKERC_SRC_DIR}/CMakeLists.txt")
    add_subdirectory("${CMAKERC_SRC_DIR}")
else()
    message(STATUS "Downloading cmakerc ${CMAKERC_VERSION}")

    FetchContent_Declare(
        cmakerc
        GIT_REPOSITORY https://github.com/vector-of-bool/cmrc.git
        GIT_TAG ${CMAKERC_VERSION}
        SOURCE_DIR ${CMAKERC_SRC_DIR}
    )

    FetchContent_MakeAvailable(cmakerc)
endif()

include(${CMAKERC_SRC_DIR}/CMakeRC.cmake)