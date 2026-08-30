#pragma once

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include <expected>
#include <graphics/vk/Image2d.hpp>
#include <string>
#include <vector>

namespace graphics::vk {

struct FrameData;

}

namespace graphics::vk::impl {

[[nodiscard]] auto draw_imgui(FrameData& frameData,  //
                              VkDevice device,  //
                              VkPipelineLayout pipelineLayout,  //
                              VkPipeline pipeline,  //
                              VkSampler sampler,  //
                              VkPhysicalDeviceProperties2 const& deviceProperties  //
                              ) noexcept -> std::expected<void, std::string>;

}  // namespace graphics::vk::impl
