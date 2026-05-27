#pragma once

#include <vulkan/vulkan.h>

#include <expected>
#include <string>

namespace graphics::vk::impl {

struct GetSwapchainImageIndexResult {
    uint32_t index;
    bool rebuildSwapchain;
};

[[nodiscard]] auto get_swapchain_image_index(VkDevice device,  //
                                             VkSwapchainKHR swapchain,  //
                                             VkSemaphore imageAvailableSemaphore  //
                                             ) noexcept -> std::expected<GetSwapchainImageIndexResult, std::string>;

}  // namespace graphics::vk::impl
