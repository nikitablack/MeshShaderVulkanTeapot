#include <fmt/core.h>

#include <graphics/vk/impl/get_swapchain_images.hpp>

namespace graphics::vk::impl {

auto get_swapchain_images(VkDevice device,  //
                          VkSwapchainKHR swapchain  //
                          ) noexcept -> std::expected<std::vector<VkImage>, std::string> {
    fmt::println("getting swapchain images");

    uint32_t imageCount{};
    if (vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr) != VK_SUCCESS) {
        return std::unexpected{"failed to get swapchain images"};
    }

    std::vector<VkImage> images(imageCount);
    if (vkGetSwapchainImagesKHR(device, swapchain, &imageCount, images.data()) != VK_SUCCESS) {
        return std::unexpected{"failed to get swapchain images"};
    }

    return images;
}

}  // namespace graphics::vk::impl