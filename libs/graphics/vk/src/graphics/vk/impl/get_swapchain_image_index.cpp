#include <graphics/vk/impl/get_swapchain_image_index.hpp>
#include <limits>
#include <vulkan/utility/vk_struct_helper.hpp>

namespace graphics::vk::impl {

auto get_swapchain_image_index(VkDevice device,  //
                               VkSwapchainKHR swapchain,  //
                               VkSemaphore imageAvailableSemaphore  //
                               ) noexcept -> std::expected<GetSwapchainImageIndexResult, std::string> {
    VkAcquireNextImageInfoKHR info = vku::InitStructHelper{};
    info.swapchain = swapchain;
    info.timeout = std::numeric_limits<uint64_t>::max();
    info.semaphore = imageAvailableSemaphore;
    info.fence = VK_NULL_HANDLE;
    info.deviceMask = 1;

    uint32_t imageIndex{};

    VkResult const result{vkAcquireNextImage2KHR(device,  //
                                                 &info,  //
                                                 &imageIndex)};

    if ((result != VK_SUCCESS) && (result != VK_SUBOPTIMAL_KHR) && (result != VK_ERROR_OUT_OF_DATE_KHR)) {
        return std::unexpected{"failed to acquire next image"};
    }

    GetSwapchainImageIndexResult imageIndexResult{};
    imageIndexResult.index = imageIndex;
    imageIndexResult.rebuildSwapchain = (result == VK_SUBOPTIMAL_KHR) || (result == VK_ERROR_OUT_OF_DATE_KHR);

    return imageIndexResult;
}

}  // namespace graphics::vk::impl
