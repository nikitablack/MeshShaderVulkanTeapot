#include <fmt/core.h>

#include <graphics/vk/impl/create_semaphore.hpp>
#include <vulkan/utility/vk_struct_helper.hpp>

namespace graphics::vk::impl {

auto create_semaphore(VkDevice device) noexcept -> std::expected<VkSemaphore, std::string> {
    fmt::println("creating semaphore");

    VkSemaphoreCreateInfo info = vku::InitStructHelper{};
    info.flags = 0;

    VkSemaphore semaphore{VK_NULL_HANDLE};
    if (vkCreateSemaphore(device, &info, nullptr, &semaphore) != VK_SUCCESS) {
        return std::unexpected{"failed to create semaphore"};
    }

    return semaphore;
}

}  // namespace graphics::vk::impl
