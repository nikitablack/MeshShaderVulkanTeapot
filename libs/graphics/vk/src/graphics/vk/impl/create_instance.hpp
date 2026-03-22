#pragma once

#include <vulkan/vulkan.h>

#include <expected>
#include <string>

namespace graphics::vk::impl {

[[nodiscard]] auto create_instance() noexcept -> std::expected<VkInstance, std::string>;

}
