#include <graphics/vk/utils/submit.hpp>
#include <vulkan/utility/vk_struct_helper.hpp>

namespace graphics::vk::utils {

auto submit(VkCommandBuffer commandBuffer,  //
            VkQueue queue,  //
            VkSemaphore imageAvailableSemaphore,  //
            VkSemaphore renderingFinishedSemaphore,  //
            VkFence fence  //
            ) noexcept -> std::expected<void, std::string> {
    VkSemaphoreSubmitInfo waitSemaphoreSubmitInfo = vku::InitStructHelper{};
    waitSemaphoreSubmitInfo.semaphore = imageAvailableSemaphore;
    waitSemaphoreSubmitInfo.value = 0;
    waitSemaphoreSubmitInfo.stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
    waitSemaphoreSubmitInfo.deviceIndex = 0;

    VkCommandBufferSubmitInfo commandBufferSubmitInfo = vku::InitStructHelper{};
    commandBufferSubmitInfo.commandBuffer = commandBuffer;
    commandBufferSubmitInfo.deviceMask = 0;

    VkSemaphoreSubmitInfo signalSemaphoreSubmitInfo = vku::InitStructHelper{};
    signalSemaphoreSubmitInfo.semaphore = renderingFinishedSemaphore;
    signalSemaphoreSubmitInfo.value = 0;
    signalSemaphoreSubmitInfo.stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
    signalSemaphoreSubmitInfo.deviceIndex = 0;

    VkSubmitInfo2 submitInfo = vku::InitStructHelper{};
    submitInfo.flags = 0;
    submitInfo.waitSemaphoreInfoCount = imageAvailableSemaphore ? 1 : 0;
    submitInfo.pWaitSemaphoreInfos = imageAvailableSemaphore ? &waitSemaphoreSubmitInfo : nullptr;
    submitInfo.commandBufferInfoCount = 1;
    submitInfo.pCommandBufferInfos = &commandBufferSubmitInfo;
    submitInfo.signalSemaphoreInfoCount = renderingFinishedSemaphore ? 1 : 0;
    submitInfo.pSignalSemaphoreInfos = renderingFinishedSemaphore ? &signalSemaphoreSubmitInfo : nullptr;

    if (vkQueueSubmit2(queue, 1, &submitInfo, fence) != VK_SUCCESS) {
        return std::unexpected{"failed to submit"};
    }

    return {};
}

}  // namespace graphics::vk::utils
