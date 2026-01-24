cmake_minimum_required(VERSION 3.25)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

set(GLFW_VERSION 3.4)
set(GLFW_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/${GLFW_VERSION}")

if(EXISTS ${GLFW_SRC_DIR}/CMakeLists.txt)
    add_subdirectory("${GLFW_SRC_DIR}")
else()
    message(STATUS "Downloading glfw ${GLFW_VERSION}")

    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG ${GLFW_VERSION}
        SOURCE_DIR ${GLFW_SRC_DIR}
    )

    FetchContent_MakeAvailable(glfw)
endif()
