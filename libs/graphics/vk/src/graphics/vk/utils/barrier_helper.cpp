#include <graphics/vk/utils/barrier_helper.hpp>
#include <vulkan/utility/vk_struct_helper.hpp>

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
                       ) noexcept -> void {
    VkImageSubresourceRange range{};
    range.aspectMask = aspect;
    range.baseMipLevel = 0;
    range.levelCount = 1;
    range.baseArrayLayer = 0;
    range.layerCount = 1;

    VkImageMemoryBarrier2 barrier = vku::InitStructHelper{};
    barrier.srcStageMask = srcStage;
    barrier.srcAccessMask = srcAccess;
    barrier.dstStageMask = dstStage;
    barrier.dstAccessMask = dstAccess;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange = range;

    VkDependencyInfo dependencyInfo = vku::InitStructHelper{};
    dependencyInfo.dependencyFlags = 0;
    dependencyInfo.memoryBarrierCount = 0;
    dependencyInfo.pMemoryBarriers = nullptr;
    dependencyInfo.bufferMemoryBarrierCount = 0;
    dependencyInfo.pBufferMemoryBarriers = nullptr;
    dependencyInfo.imageMemoryBarrierCount = 1;
    dependencyInfo.pImageMemoryBarriers = &barrier;

    vkCmdPipelineBarrier2(commandBuffer, &dependencyInfo);
}

}  // namespace graphics::vk::utils
