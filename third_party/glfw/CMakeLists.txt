cmake_minimum_required(VERSION 3.25)

set(GLFW_VERSION 3.4)
message(STATUS "Downloading glfw ${GLFW_VERSION}")

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG ${GLFW_VERSION}
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src
)

FetchContent_MakeAvailable(glfw)
