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
        ${CMAKE_CURRENT_SOURCE_DIR}/include/graphics/vk/utils/barrier_helper.hpp

        ${CMAKE_CURRENT_SOURCE_DIR}/include/graphics/vk/GraphicsManager.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/graphics/vk/Image2d.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/graphics/vk/VulkanQueue.hpp

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

        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/allocate_command_buffer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/allocate_command_buffer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/begin_compose_rendering.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/begin_compose_rendering.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/check_instance_version.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/check_instance_version.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/check_physical_device_support.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/check_physical_device_support.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/check_required_instance_extension.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/check_required_instance_extension.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/create_allocator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/create_allocator.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/create_command_pool.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/create_command_pool.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/create_device.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/create_device.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/create_fence.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/create_fence.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/create_instance.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/create_instance.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/create_semaphore.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/create_semaphore.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/create_surface.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/create_surface.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/create_swapchain_image_views.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/create_swapchain_image_views.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/create_swapchain.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/create_swapchain.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/get_graphics_queue_family.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/get_graphics_queue_family.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/get_physical_device_properties.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/get_physical_device_properties.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/get_present_mode.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/get_present_mode.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/get_queue.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/get_queue.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/get_supported_physical_devices.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/get_supported_physical_devices.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/get_surface_capabilities.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/get_surface_capabilities.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/get_surface_extent.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/get_surface_extent.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/get_swapchain_images.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/get_swapchain_images.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/get_surface_format.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/get_surface_format.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/get_swapchain_image_index.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/get_swapchain_image_index.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/present.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/present.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/RequiredApiVersion.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/RequiredDeviceExtensions.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/RequiredDeviceExtensions.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/RequiredInstanceExtensions.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/RequiredInstanceExtensions.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/submit.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/impl/submit.hpp

        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/utils/barrier_helper.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/GraphicsManager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/vk/Image2d.cpp
)

target_include_directories(graphics_vk
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set(shaders "shaders")
cmrc_add_resource_library(${shaders} "")

add_library(libs::shaders
    ALIAS
    shaders
)

addCompileShadersCommand(
    TARGET_NAME ${shaders}
    OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/shaders
    SHADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/fullscreen_triangle.frag
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/fullscreen_triangle.vert
)

target_link_libraries(graphics_vk
    PUBLIC
        Vulkan::Vulkan
        libs::window
        GPUOpen::VulkanMemoryAllocator

    PRIVATE
        fmt::fmt
        libs::shaders
        libs::utils
        libs::warnings_as_errors
)
