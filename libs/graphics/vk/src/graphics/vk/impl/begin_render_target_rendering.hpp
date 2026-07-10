#pragma once

#include <vulkan/vulkan.h>

#include <array>
#include <graphics/vk/Image2d.hpp>

namespace graphics::vk::impl {

auto begin_render_target_rendering(VkCommandBuffer commandBuffer,  //
                                   Image2d const& renderTarget,  //
                                   std::array<float, 4> const& clearColorRgba,  //
                                   VkExtent2D const& surfaceExtent  //
                                   ) noexcept -> void;

}
