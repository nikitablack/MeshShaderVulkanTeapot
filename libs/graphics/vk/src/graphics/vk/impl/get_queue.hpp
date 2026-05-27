#pragma once

#include <vulkan/vulkan.h>

namespace graphics::vk::impl {

auto get_queue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex) -> VkQueue;

}
