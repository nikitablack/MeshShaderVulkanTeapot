#pragma once

#include <vulkan/vulkan.h>

#include <expected>
#include <string>

namespace graphics::vk::impl {

[[nodiscard]] auto create_command_pool(VkDevice device,  //
                                       uint32_t queueFamilyIndex  //
                                       ) noexcept -> std::expected<VkCommandPool, std::string>;

}
