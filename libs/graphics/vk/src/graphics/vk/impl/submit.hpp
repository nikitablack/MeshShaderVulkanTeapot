#pragma once

#include <vulkan/vulkan.h>

#include <expected>
#include <string>

namespace graphics::vk::impl {

[[nodiscard]] auto submit(VkCommandBuffer commandBuffer,  //
                          VkQueue queue,  //
                          VkSemaphore imageAvailableSemaphore,  //
                          VkSemaphore renderingFinishedSemaphore  //
                          ) noexcept -> std::expected<void, std::string>;

}
