#pragma once

#include <vulkan/vulkan.h>

namespace graphics::vk {

class FrameData;
class Image2d;

}  // namespace graphics::vk

namespace graphics::vk::impl {

auto draw_fullscreen_triangle(VkDevice device,  //
                              VkPipeline pipeline,  //
                              VkPipelineLayout pipelineLayout,  //
                              VkSampler renderTargetSampler,  //
                              Image2d const& renderTarget,  //
                              VkExtent2D const& surfaceExtent,  //
                              FrameData& frameData  //
                              ) noexcept -> void;

}  // namespace graphics::vk::impl
