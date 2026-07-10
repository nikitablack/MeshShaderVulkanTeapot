#pragma once

#include <vulkan/vulkan.h>

#include <expected>
#include <string>

namespace graphics::vk::impl {

[[nodiscard]] auto allocate_descriptor_set(VkDevice device,  //
                                           VkDescriptorPool descriptorPool,  //
                                           VkDescriptorSetLayout descriptorSetLayout,  //
                                           uint32_t descriptorCount  //
                                           ) noexcept -> std::expected<VkDescriptorSet, std::string>;

}
