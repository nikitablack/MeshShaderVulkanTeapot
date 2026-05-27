#pragma once

#include <vulkan/vulkan.h>

#include <expected>
#include <string>

namespace graphics::vk::impl {

[[nodiscard]] auto present(VkQueue queue,  //
                           VkSwapchainKHR swapchain,  //
                           uint32_t swapchainImageIndex,  //
                           VkSemaphore renderingFinishedSemaphore  //
                           ) noexcept -> std::expected<void, std::string>;

}
