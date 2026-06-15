#include <fmt/core.h>

#include <graphics/vk/impl/create_fence.hpp>
#include <vulkan/utility/vk_struct_helper.hpp>

namespace graphics::vk::impl {

auto create_fence(VkDevice device) noexcept -> std::expected<VkFence, std::string> {
    fmt::println("creating fence");

    VkFenceCreateInfo info = vku::InitStructHelper{};
    info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    VkFence fence{VK_NULL_HANDLE};
    if (vkCreateFence(device, &info, nullptr, &fence) != VK_SUCCESS) {
        return std::unexpected{"failed to create fence"};
    }

    return fence;
}

}  // namespace graphics::vk::impl
