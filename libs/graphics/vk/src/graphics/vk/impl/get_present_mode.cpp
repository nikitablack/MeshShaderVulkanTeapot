#include <fmt/core.h>

#include <algorithm>
#include <graphics/vk/impl/get_present_mode.hpp>
#include <vector>

namespace graphics::vk::impl {

auto get_present_mode(VkPhysicalDevice physicalDevice,  //
                      VkSurfaceKHR surface  //
                      ) noexcept -> std::expected<VkPresentModeKHR, std::string> {
    fmt::println("getting present mode");

    uint32_t modeCount{0};
    if ((vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &modeCount, nullptr) != VK_SUCCESS) ||
        (modeCount == 0)) {
        return std::unexpected{"failed to get present mode"};
    }

    std::vector<VkPresentModeKHR> modes(modeCount);
    if (vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &modeCount, modes.data()) != VK_SUCCESS) {
        return std::unexpected{"failed to get present mode"};
    }

    if (std::find(modes.begin(), modes.end(), VK_PRESENT_MODE_MAILBOX_KHR) != modes.end()) {
        return VK_PRESENT_MODE_MAILBOX_KHR;
    }

    return VK_PRESENT_MODE_FIFO_KHR;  // guaranteed to be supported
}

}  // namespace graphics::vk::impl