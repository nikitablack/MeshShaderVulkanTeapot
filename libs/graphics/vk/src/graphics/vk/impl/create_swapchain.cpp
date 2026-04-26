#include <fmt/core.h>

#include <algorithm>
#include <graphics/vk/impl/create_swapchain.hpp>
#include <vulkan/utility/vk_struct_helper.hpp>

namespace graphics::vk::impl {

auto create_swapchain(VkDevice device,  //
                      VkSurfaceKHR surface,  //
                      VkSurfaceFormat2KHR surfaceFormat,  //
                      VkPresentModeKHR presentMode,  //
                      VkSurfaceCapabilities2KHR const& surfaceCapabilities,  //
                      VkExtent2D surfaceExtent,  //
                      VkSwapchainKHR oldSwapchain  //
                      ) noexcept -> std::expected<VkSwapchainKHR, std::string> {
    fmt::println("creating swapchain");

    uint32_t constexpr IMAGE_COUNT{3};

    auto const m{std::max(surfaceCapabilities.surfaceCapabilities.minImageCount, IMAGE_COUNT)};
    auto const imageCount{std::min(m, surfaceCapabilities.surfaceCapabilities.maxImageCount)};

    VkSwapchainCreateInfoKHR info{};
    info.sType = vku::GetSType<VkSwapchainCreateInfoKHR>();
    info.pNext = nullptr;
    info.flags = 0;
    info.surface = surface;
    info.minImageCount = imageCount;
    info.imageFormat = surfaceFormat.surfaceFormat.format;
    info.imageColorSpace = surfaceFormat.surfaceFormat.colorSpace;
    info.imageExtent = surfaceExtent;
    info.imageArrayLayers = 1;
    info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    info.queueFamilyIndexCount = 0;
    info.pQueueFamilyIndices = nullptr;
    info.preTransform = surfaceCapabilities.surfaceCapabilities.currentTransform;
    info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    info.presentMode = presentMode;
    info.clipped = VK_TRUE;
    info.oldSwapchain = oldSwapchain;

    VkSwapchainKHR swapchain{VK_NULL_HANDLE};
    if (vkCreateSwapchainKHR(device, &info, nullptr, &swapchain) != VK_SUCCESS) {
        return std::unexpected{"failed to create swapchain"};
    }

    if (oldSwapchain) {
        vkDestroySwapchainKHR(device, oldSwapchain, nullptr);
    }

    return swapchain;
}

}  // namespace graphics::vk::impl
