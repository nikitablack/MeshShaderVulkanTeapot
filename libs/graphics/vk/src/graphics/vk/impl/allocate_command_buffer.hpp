#pragma once

#include <vulkan/vulkan.h>

#include <expected>
#include <string>

namespace graphics::vk::impl {

[[nodiscard]] auto allocate_command_buffer(VkDevice device,  //
                                           VkCommandPool commandPool  //
                                           ) noexcept -> std::expected<VkCommandBuffer, std::string>;

}
