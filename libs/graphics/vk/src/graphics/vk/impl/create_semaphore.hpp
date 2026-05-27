#pragma once

#include <vulkan/vulkan.h>

#include <expected>
#include <string>

namespace graphics::vk::impl {

[[nodiscard]] auto create_semaphore(VkDevice device) noexcept -> std::expected<VkSemaphore, std::string>;

}
