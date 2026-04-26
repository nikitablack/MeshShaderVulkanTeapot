#pragma once

#include <vulkan/vulkan.h>

#include <expected>
#include <string>

namespace graphics::vk::impl {

[[nodiscard]] auto create_swapchain(VkDevice device,  //
                                    VkSurfaceKHR surface,  //
                                    VkSurfaceFormat2KHR surfaceFormat,  //
                                    VkPresentModeKHR presentMode,  //
                                    VkSurfaceCapabilities2KHR const& surfaceCapabilities,  //
                                    VkExtent2D surfaceExtent,  //
                                    VkSwapchainKHR oldSwapchain  //
                                    ) noexcept -> std::expected<VkSwapchainKHR, std::string>;

}
