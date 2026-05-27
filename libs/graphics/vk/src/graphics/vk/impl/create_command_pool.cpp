#include <fmt/core.h>

#include <graphics/vk/impl/create_command_pool.hpp>
#include <vulkan/utility/vk_struct_helper.hpp>

namespace graphics::vk::impl {

auto create_command_pool(VkDevice device,  //
                         uint32_t queueFamilyIndex  //
                         ) noexcept -> std::expected<VkCommandPool, std::string> {
    fmt::println("creating command pool");

    VkCommandPoolCreateInfo info = vku::InitStructHelper{};
    info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
    info.queueFamilyIndex = queueFamilyIndex;

    VkCommandPool commandPool{VK_NULL_HANDLE};
    if (vkCreateCommandPool(device, &info, nullptr, &commandPool) != VK_SUCCESS) {
        return std::unexpected("failed to create command pool");
    }

    return commandPool;
}

}  // namespace graphics::vk::impl
