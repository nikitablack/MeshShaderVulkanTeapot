#include <graphics/vk/Buffer.hpp>
#include <graphics/vk/HostVisibleBuffer.hpp>
#include <graphics/vk/Image2d.hpp>
#include <graphics/vk/VulkanQueue.hpp>
#include <graphics/vk/impl/allocate_command_buffer.hpp>
#include <graphics/vk/utils/barrier_helper.hpp>
#include <graphics/vk/utils/init_helper.hpp>
#include <graphics/vk/utils/submit.hpp>
#include <utils/try_expected.hpp>
#include <vulkan/utility/vk_struct_helper.hpp>

namespace graphics::vk::utils {

auto init_buffer_sync(VmaAllocator allocator,  //
                      VkCommandBuffer commandBuffer,  //
                      Buffer const& buffer,  //
                      std::span<std::byte const> initData,  //
                      VkPipelineStageFlags2 dstStage,  //
                      VkAccessFlags2 dstAccess,  //
                      VulkanQueue const& queue  //
                      ) noexcept -> std::expected<void, std::string> {
    size_t const sizeBytes{initData.size_bytes()};

    if (buffer.size() < sizeBytes) {
        return std::unexpected{"buffer size is too small"};
    }

    HostVisibleBuffer stagingBuffer{};
    TRY_EXPECTED_VOID(stagingBuffer.init(allocator, sizeBytes));
    TRY_EXPECTED_VOID(stagingBuffer.copyToBuffer(initData));

    VkBufferCopy2 region = vku::InitStructHelper{};
    region.srcOffset = 0;
    region.dstOffset = 0;
    region.size = sizeBytes;

    VkCopyBufferInfo2 copyInfo = vku::InitStructHelper{};
    copyInfo.srcBuffer = stagingBuffer.buffer();
    copyInfo.dstBuffer = buffer.buffer();
    copyInfo.regionCount = 1;
    copyInfo.pRegions = &region;

    vkCmdCopyBuffer2(commandBuffer, &copyInfo);

    set_buffer_barrier(commandBuffer,  //
                       buffer.buffer(),  //
                       0,  //
                       sizeBytes,  //
                       VK_PIPELINE_STAGE_2_TRANSFER_BIT,  //
                       VK_ACCESS_2_TRANSFER_WRITE_BIT,  //
                       dstStage,  //
                       dstAccess);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        return std::unexpected{"failed to end command buffer"};
    }

    TRY_EXPECTED_VOID(submit(commandBuffer,  //
                             queue.queue,  //
                             VK_NULL_HANDLE,  //
                             VK_NULL_HANDLE,  //
                             VK_NULL_HANDLE));

    if (vkQueueWaitIdle(queue.queue) != VK_SUCCESS) {
        return std::unexpected{"failed to wait queue"};
    }

    stagingBuffer.destroy();

    return {};
}

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
                     ) noexcept -> std::expected<void, std::string> {
    size_t const sizeBytes{initData.size_bytes()};

    if (image.size() < sizeBytes) {
        return std::unexpected{"image size is too small"};
    }

    TRY_EXPECTED(auto const commandBuffer, impl::allocate_command_buffer(device, commandPool));

    HostVisibleBuffer stagingBuffer{};
    TRY_EXPECTED_VOID(stagingBuffer.init(allocator, sizeBytes));
    TRY_EXPECTED_VOID(stagingBuffer.copyToBuffer(initData));

    VkImageSubresourceLayers imageSubresource{};
    imageSubresource.aspectMask = image.aspectFlags();
    imageSubresource.mipLevel = 0;
    imageSubresource.baseArrayLayer = 0;
    imageSubresource.layerCount = 1;

    VkOffset3D imageOffset{};
    imageOffset.x = 0;
    imageOffset.y = 0;
    imageOffset.z = 0;

    VkExtent3D imageExtent{};
    imageExtent.width = image.width();
    imageExtent.height = image.height();
    imageExtent.depth = 1;

    VkBufferImageCopy2 region = vku::InitStructHelper{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource = imageSubresource;
    region.imageOffset = imageOffset;
    region.imageExtent = imageExtent;

    VkCopyBufferToImageInfo2 copyInfo = vku::InitStructHelper{};
    copyInfo.srcBuffer = stagingBuffer.buffer();
    copyInfo.dstImage = image.image();
    copyInfo.dstImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    copyInfo.regionCount = 1;
    copyInfo.pRegions = &region;

    set_image_barrier(commandBuffer,  //
                      image.image(),  //
                      image.aspectFlags(),  //
                      srcStage,  //
                      srcAccess,  //
                      VK_PIPELINE_STAGE_2_TRANSFER_BIT,  //
                      VK_ACCESS_2_TRANSFER_WRITE_BIT,  //
                      VK_IMAGE_LAYOUT_UNDEFINED,  //
                      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    vkCmdCopyBufferToImage2(commandBuffer, &copyInfo);

    set_image_barrier(commandBuffer,  //
                      image.image(),  //
                      image.aspectFlags(),  //
                      VK_PIPELINE_STAGE_2_TRANSFER_BIT,  //
                      VK_ACCESS_2_TRANSFER_WRITE_BIT,  //
                      dstStage,  //
                      dstAccess,  //
                      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,  //
                      dstLayout);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        return std::unexpected{"failed to end command buffer"};
    }

    TRY_EXPECTED_VOID(submit(commandBuffer,  //
                             queue,  //
                             VK_NULL_HANDLE,  //
                             VK_NULL_HANDLE,  //
                             VK_NULL_HANDLE));

    if (vkQueueWaitIdle(queue) != VK_SUCCESS) {
        return std::unexpected{"failed to wait queue"};
    }

    stagingBuffer.destroy();

    return {};
}

}  // namespace graphics::vk::utils