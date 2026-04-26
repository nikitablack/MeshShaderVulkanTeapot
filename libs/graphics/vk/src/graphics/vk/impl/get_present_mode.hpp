#pragma once

#include <vulkan/vulkan.h>

#include <expected>
#include <string>

namespace graphics::vk::impl {

[[nodiscard]] auto get_present_mode(VkPhysicalDevice physicalDevice,  //
                                    VkSurfaceKHR surface  //
                                    ) noexcept -> std::expected<VkPresentModeKHR, std::string>;

}
