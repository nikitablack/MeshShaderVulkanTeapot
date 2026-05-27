#pragma once

#include <vulkan/vulkan.h>

namespace graphics::vk::impl {

auto begin_compose_rendering(VkCommandBuffer commandBuffer,  //
                             VkImage swapchainImage,  //
                             VkImageView swapchainImageView,  //
                             VkExtent2D const& surfaceExtent  //
                             ) noexcept -> void;

}
