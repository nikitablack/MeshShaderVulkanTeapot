#pragma once

#include <vulkan/vulkan.h>

#include <graphics/vk/HostVisibleBuffer.hpp>

namespace graphics::vk {

struct FrameData {
    VkCommandBuffer commandBuffer{VK_NULL_HANDLE};
    VkDescriptorSet storageDescriptorSet{VK_NULL_HANDLE};
    VkDescriptorSet cisDescriptorSet{VK_NULL_HANDLE};
    uint32_t storageDescriptorCounter{0};
    uint32_t cisDescriptorCounter{0};
    HostVisibleBuffer* imguiBuffer{};
};

}  // namespace graphics::vk