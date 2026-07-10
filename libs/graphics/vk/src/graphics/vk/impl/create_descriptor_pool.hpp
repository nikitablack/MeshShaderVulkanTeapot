#pragma once

#include <vulkan/vulkan.h>

#include <expected>
#include <string>

namespace graphics::vk::impl {

[[nodiscard]] auto create_descriptor_pool(VkDevice device,  //
                                          uint32_t descriptorCount  //
                                          ) noexcept -> std::expected<VkDescriptorPool, std::string>;

}
