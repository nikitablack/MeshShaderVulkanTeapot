#pragma once

#include <vulkan/vulkan.h>

namespace window {

class Window;

}

namespace graphics::vk::impl {

auto get_surface_extent(VkSurfaceCapabilities2KHR const& surfaceCapabilities,  //
                        window::Window& window) noexcept -> VkExtent2D;

}
