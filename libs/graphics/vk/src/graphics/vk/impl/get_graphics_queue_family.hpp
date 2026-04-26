#pragma once

#include <vulkan/vulkan.h>

#include <expected>
#include <string>

namespace graphics::vk::impl {

[[nodiscard]] auto get_graphics_queue_family(VkPhysicalDevice physicalDevice,  //
                                             VkSurfaceKHR surface,  //
                                             uint32_t requiredQueueCount  //
                                             ) noexcept -> std::expected<uint32_t, std::string>;

}
