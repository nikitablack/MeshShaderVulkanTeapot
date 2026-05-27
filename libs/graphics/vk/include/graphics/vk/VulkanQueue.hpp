#pragma once

#include <vulkan/vulkan.h>

namespace graphics::vk {

struct VulkanQueue {
    uint32_t familyIndex;
    VkQueue queue;
};

}  // namespace graphics::vk
