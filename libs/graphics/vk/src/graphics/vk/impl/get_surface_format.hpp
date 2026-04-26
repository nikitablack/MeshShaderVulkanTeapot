#pragma once

#include <vulkan/vulkan.h>

#include <expected>
#include <string>

namespace graphics::vk::impl {

[[nodiscard]] auto get_surface_format(VkPhysicalDevice physicalDevice,  //
                                      VkSurfaceKHR surface  //
                                      ) noexcept -> std::expected<VkSurfaceFormat2KHR, std::string>;

}
