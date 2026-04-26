#pragma once

#include <vulkan/vulkan.h>

#include <expected>
#include <string>
#include <vector>

namespace graphics::vk::impl {

[[nodiscard]] auto get_swapchain_images(VkDevice device,  //
                                        VkSwapchainKHR swapchain  //
                                        ) noexcept -> std::expected<std::vector<VkImage>, std::string>;

}
