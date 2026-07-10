#pragma once

#include <vulkan/vulkan.h>

#include <expected>
#include <string>

namespace graphics::vk::impl {

[[nodiscard]] auto create_fullscreen_triangle_pipeline(VkDevice device,  //
                                                       VkPipelineLayout pipelineLayout,  //
                                                       VkFormat surfaceFormat  //
                                                       ) noexcept -> std::expected<VkPipeline, std::string>;

}  // namespace graphics::vk::impl
