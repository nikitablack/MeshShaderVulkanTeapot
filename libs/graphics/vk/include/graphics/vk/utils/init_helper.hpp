#pragma once

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include <expected>
#include <span>
#include <string>

namespace graphics::vk {

class Buffer;
class Image2d;
struct VulkanQueue;

}  // namespace graphics::vk

namespace graphics::vk::utils {

auto init_buffer_sync(VmaAllocator allocator,  //
                      VkCommandBuffer commandBuffer,  //
                      Buffer const& buffer,  //
                      std::span<std::byte const> initData,  //
                      VkPipelineStageFlags2 dstStage,  //
                      VkAccessFlags2 dstAccess,  //
                      VulkanQueue const& queue  //
                      ) noexcept -> std::expected<void, std::string>;

auto init_image_sync(VkDevice device,  //
                     VmaAllocator allocator,  //
                     VkCommandPool commandPool,  //
                     Image2d const& image,  //
                     std::span<std::byte const> initData,  //
                     VkPipelineStageFlags2 srcStage,  //
                     VkAccessFlags2 srcAccess,  //
                     VkPipelineStageFlags2 dstStage,  //
                     VkAccessFlags2 dstAccess,  //
                     VkImageLayout dstLayout,  //
                     VkQueue queue  //
                     ) noexcept -> std::expected<void, std::string>;

}  // namespace graphics::vk::utils