#include <fmt/core.h>

#include <graphics/vk/impl/get_graphics_queue_family.hpp>
#include <vector>
#include <vulkan/utility/vk_struct_helper.hpp>

namespace graphics::vk::impl {

auto get_graphics_queue_family(VkPhysicalDevice physicalDevice,  //
                               VkSurfaceKHR surface,  //
                               uint32_t requiredQueueCount  //
                               ) noexcept -> std::expected<uint32_t, std::string> {
    fmt::println("getting graphics queue family");

    uint32_t queueFamilyCount{};
    vkGetPhysicalDeviceQueueFamilyProperties2(physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties2> queueFamilies(queueFamilyCount);
    for (size_t i{0}; i < queueFamilies.size(); ++i) {
        queueFamilies[i].sType = vku::GetSType<VkQueueFamilyProperties2>();
    }

    vkGetPhysicalDeviceQueueFamilyProperties2(physicalDevice, &queueFamilyCount, queueFamilies.data());

    for (size_t i{0}; i < queueFamilies.size(); ++i) {
        auto const family{queueFamilies[i]};

        if ((family.queueFamilyProperties.queueCount >= requiredQueueCount) &&
            (family.queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
            VkBool32 presentSupported{VK_FALSE};

            if (vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice,  //
                                                     static_cast<uint32_t>(i),  //
                                                     surface,  //
                                                     &presentSupported) != VK_SUCCESS) {
                continue;
            }

            if (presentSupported) {
                return static_cast<uint32_t>(i);
            }
        }
    }

    return std::unexpected{"failed to find graphics queue"};
}

}  // namespace graphics::vk::impl
