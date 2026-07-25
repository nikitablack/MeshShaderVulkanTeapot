#pragma once

#include <vulkan/vulkan.h>

#include <expected>
#include <string>

namespace graphics::vk::utils {

[[nodiscard]] auto submit(VkCommandBuffer commandBuffer,  //
                          VkQueue queue,  //
                          VkSemaphore imageAvailableSemaphore,  //
                          VkSemaphore renderingFinishedSemaphore,  //
                          VkFence fence  //
                          ) noexcept -> std::expected<void, std::string>;

}
