cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

include(FetchContent)

add_subdirectory(libs/graphics/vk)
add_subdirectory(libs/utils)
add_subdirectory(libs/warnings)
add_subdirectory(libs/window)

add_subdirectory(third_party/fmt)
add_subdirectory(third_party/glfw)

add_subdirectory(app)
