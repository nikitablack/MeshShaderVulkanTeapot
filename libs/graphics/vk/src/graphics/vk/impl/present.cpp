#include <graphics/vk/impl/present.hpp>
#include <vulkan/utility/vk_struct_helper.hpp>

namespace graphics::vk::impl {

auto present(VkQueue queue,  //
             VkSwapchainKHR swapchain,  //
             uint32_t swapchainImageIndex,  //
             VkSemaphore renderingFinishedSemaphore  //
             ) noexcept -> std::expected<void, std::string> {
    VkPresentInfoKHR info = vku::InitStructHelper{};
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores = &renderingFinishedSemaphore;
    info.swapchainCount = 1;
    info.pSwapchains = &swapchain;
    info.pImageIndices = &swapchainImageIndex;
    info.pResults = nullptr;

    if (auto const result{vkQueuePresentKHR(queue, &info)};
        (result != VK_SUCCESS) && (result != VK_SUBOPTIMAL_KHR) && (result != VK_ERROR_OUT_OF_DATE_KHR)) {
        return std::unexpected{"failed to present"};
    }

    return {};
}

}  // namespace graphics::vk::impl
