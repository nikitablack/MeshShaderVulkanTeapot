#pragma once

#include <vulkan/vulkan.h>

#include <expected>
#include <string>

namespace graphics::vk::impl {

[[nodiscard]] auto create_device(VkPhysicalDevice physicalDevice,  //
                                 uint32_t queueFamily,  //
                                 uint32_t queueCount  //
                                 ) noexcept -> std::expected<VkDevice, std::string>;

}
