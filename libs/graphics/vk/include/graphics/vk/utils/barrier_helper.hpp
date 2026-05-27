#pragma once

#include <vulkan/vulkan.h>

namespace graphics::vk::utils {

auto set_image_barrier(VkCommandBuffer commandBuffer,  //
                       VkImage image,  //
                       VkImageAspectFlags aspect,  //
                       VkPipelineStageFlags2 srcStage,  //
                       VkAccessFlags2 srcAccess,  //
                       VkPipelineStageFlags2 dstStage,  //
                       VkAccessFlags2 dstAccess,  //
                       VkImageLayout oldLayout,  //
                       VkImageLayout newLayout  //
                       ) noexcept -> void;

}
