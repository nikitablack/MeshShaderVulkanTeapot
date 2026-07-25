cmake_minimum_required(VERSION 3.25)

find_package(Vulkan REQUIRED)

set(IMGUI_VERSION v1.92.8)
set(IMGUI_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/${IMGUI_VERSION}")

if(NOT EXISTS "${IMGUI_SRC_DIR}/LICENSE.txt")
    message(STATUS "Downloading imgui ${IMGUI_VERSION}")

    FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG ${IMGUI_VERSION}
        SOURCE_DIR ${IMGUI_SRC_DIR}
    )

    FetchContent_MakeAvailable(imgui)
endif()

add_library(imgui STATIC "")

target_sources(imgui
    PRIVATE
        ${IMGUI_SRC_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_SRC_DIR}/misc/cpp/imgui_stdlib.cpp
        ${IMGUI_SRC_DIR}/imgui.cpp
        ${IMGUI_SRC_DIR}/imgui_demo.cpp
        ${IMGUI_SRC_DIR}/imgui_draw.cpp
        ${IMGUI_SRC_DIR}/imgui_tables.cpp
        ${IMGUI_SRC_DIR}/imgui_widgets.cpp
)

target_include_directories(imgui
    PUBLIC
        ${IMGUI_SRC_DIR}
)

target_compile_definitions(imgui
    PUBLIC
        IMGUI_DISABLE_OBSOLETE_FUNCTIONS
        IMGUI_USER_CONFIG="${CMAKE_CURRENT_LIST_DIR}/cmake/imconfig.h"
)

set_target_properties(imgui
    PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_link_libraries(imgui
    PRIVATE
        glfw
        Vulkan::Vulkan
)
