#pragma once

#include <vulkan/vulkan.h>

namespace graphics::vk::impl {

[[nodiscard]] auto get_physical_device_properties(VkPhysicalDevice device) noexcept -> VkPhysicalDeviceProperties2;

}
