#pragma once

#include <vulkan/vulkan.h>

#include <expected>
#include <string>
#include <vector>

namespace graphics::vk::impl {

[[nodiscard]] auto get_supported_physical_devices(VkInstance instance  //
                                                  ) noexcept
    -> std::expected<std::vector<VkPhysicalDevice>, std::string>;

}
