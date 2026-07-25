cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(teapot LANGUAGES CXX)

include(FetchContent)
include(libs/cmake/CompileVulkanShaders.cmake)

add_subdirectory(third_party/cmakerc SYSTEM)
add_subdirectory(third_party/fmt SYSTEM)
add_subdirectory(third_party/glfw SYSTEM)
add_subdirectory(third_party/imgui SYSTEM)
add_subdirectory(third_party/vma SYSTEM)

add_subdirectory(libs/graphics/vk)
add_subdirectory(libs/utils)
add_subdirectory(libs/warnings)
add_subdirectory(libs/window)

add_subdirectory(app)
