#pragma once

#include <vulkan/vulkan.h>

#include <expected>
#include <string>

namespace graphics::vk::impl {

[[nodiscard]] auto get_surface_capabilities(VkPhysicalDevice physicalDevice,  //
                                            VkSurfaceKHR surface  //
                                            ) noexcept -> std::expected<VkSurfaceCapabilities2KHR, std::string>;

}
