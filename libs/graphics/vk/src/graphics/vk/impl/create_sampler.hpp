#pragma once

#include <vulkan/vulkan.h>

#include <expected>
#include <string>

namespace graphics::vk::impl {

[[nodiscard]] auto create_sampler(VkDevice device) noexcept -> std::expected<VkSampler, std::string>;

}  // namespace graphics::vk::impl
