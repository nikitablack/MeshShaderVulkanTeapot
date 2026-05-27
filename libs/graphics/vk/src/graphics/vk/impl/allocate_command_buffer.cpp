#include <graphics/vk/impl/allocate_command_buffer.hpp>
#include <vulkan/utility/vk_struct_helper.hpp>

namespace graphics::vk::impl {

auto allocate_command_buffer(VkDevice device,  //
                             VkCommandPool commandPool  //
                             ) noexcept -> std::expected<VkCommandBuffer, std::string> {
    VkCommandBufferAllocateInfo allocateInfo = vku::InitStructHelper{};
    allocateInfo.commandPool = commandPool;
    allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocateInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer{VK_NULL_HANDLE};
    if (vkAllocateCommandBuffers(device, &allocateInfo, &commandBuffer) != VK_SUCCESS) {
        return std::unexpected{"failed to allocate command buffer"};
    }

    VkCommandBufferBeginInfo beginInfo = vku::InitStructHelper{};
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    beginInfo.pInheritanceInfo = nullptr;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        return std::unexpected{"failed to begin command buffer"};
    }

    return commandBuffer;
}

}  // namespace graphics::vk::impl