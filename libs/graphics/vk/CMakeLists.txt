cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

find_package(Vulkan REQUIRED)

add_library(graphics_vk STATIC "")

add_library(libs::graphics::vk
    ALIAS
        graphics_vk
)

set_target_properties(graphics_vk
    PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_sources(graphics_vk
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/graphics/vk/GraphicsManager.hpp

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/features/Features.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/features/MeshShaderFeatures.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/features/MeshShaderFeatures.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/features/RequiredFeatures.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/features/RequiredFeatures.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/features/Vulkan12Features.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/features/Vulkan12Features.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/features/Vulkan13Features.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/features/Vulkan13Features.hpp

        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/check_instance_version.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/check_instance_version.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/check_physical_device_support.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/check_physical_device_support.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/check_required_instance_extension.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/check_required_instance_extension.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/create_instance.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/create_instance.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/create_surface.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/create_surface.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/get_physical_device_properties.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/get_physical_device_properties.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/get_supported_physical_devices.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/get_supported_physical_devices.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/RequiredApiVersion.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/RequiredDeviceExtensions.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/RequiredDeviceExtensions.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/RequiredInstanceExtensions.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/RequiredInstanceExtensions.hpp

        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/GraphicsManager.cpp
)

target_include_directories(graphics_vk
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(graphics_vk
    PUBLIC
        Vulkan::Vulkan
        libs::window

    PRIVATE
        fmt::fmt
        libs::utils
        libs::warnings_as_errors
)
