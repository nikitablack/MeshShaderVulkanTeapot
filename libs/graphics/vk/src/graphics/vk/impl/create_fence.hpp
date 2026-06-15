#pragma once

#include <vulkan/vulkan.h>

#include <expected>
#include <string>

namespace graphics::vk::impl {

[[nodiscard]] auto create_fence(VkDevice device) noexcept -> std::expected<VkFence, std::string>;

}
