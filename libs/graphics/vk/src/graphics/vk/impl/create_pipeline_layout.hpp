#pragma once

#include <vulkan/vulkan.h>

#include <expected>
#include <string>

namespace graphics::vk::impl {

[[nodiscard]] auto create_pipeline_layout(VkDevice device,  //
                                          VkDescriptorSetLayout storageDescriptorSetLayout,  //
                                          VkDescriptorSetLayout cisDescriptorSetLayout  //
                                          ) noexcept -> std::expected<VkPipelineLayout, std::string>;

}
