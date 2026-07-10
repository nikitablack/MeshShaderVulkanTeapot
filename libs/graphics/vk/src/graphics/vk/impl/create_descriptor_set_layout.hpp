#pragma once

#include <vulkan/vulkan.h>

#include <expected>
#include <string>

namespace graphics::vk::impl {

[[nodiscard]] auto create_descriptor_set_layout(VkDevice device,  //
                                                VkDescriptorType descriptorType,  //
                                                uint32_t maxRequiredDescriptorCount  //
                                                ) noexcept -> std::expected<VkDescriptorSetLayout, std::string>;

}
