#pragma once

#include <vulkan/vulkan.h>

#include <graphics/vk/Image2d.hpp>

namespace graphics::vk::impl {

auto begin_compose_rendering(VkCommandBuffer commandBuffer,  //
                             Image2d const& renderTarget,  //
                             VkImage swapchainImage,  //
                             VkImageView swapchainImageView,  //
                             VkExtent2D const& surfaceExtent  //
                             ) noexcept -> void;

}
