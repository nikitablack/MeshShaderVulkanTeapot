#include <fmt/core.h>

#include <algorithm>
#include <graphics/vk/impl/RequiredDeviceExtensions.hpp>
#include <graphics/vk/impl/create_device.hpp>
#include <graphics/vk/impl/features/RequiredFeatures.hpp>
#include <vector>
#include <vulkan/utility/vk_struct_helper.hpp>

namespace graphics::vk::impl {

auto create_device(VkPhysicalDevice physicalDevice,  //
                   uint32_t queueFamily,  //
                   uint32_t queueCount  //
                   ) noexcept -> std::expected<VkDevice, std::string> {
    fmt::println("creating device");

    std::vector<float> queuePriorities(queueCount);
    std::fill(queuePriorities.begin(), queuePriorities.end(), 1.0f);

    VkDeviceQueueCreateInfo queueCreateInfo = vku::InitStructHelper{};
    queueCreateInfo.flags = 0;
    queueCreateInfo.queueFamilyIndex = queueFamily;
    queueCreateInfo.queueCount = static_cast<uint32_t>(queuePriorities.size());
    queueCreateInfo.pQueuePriorities = queuePriorities.data();

    auto const requiredExtensions{RequiredDeviceExtensions::get()};

    std::vector<char const*> extensionsStr{};
    extensionsStr.reserve(requiredExtensions.size());

    for (auto const& ext : requiredExtensions) {
        extensionsStr.push_back(ext.data());
    }

    features::RequiredFeatures requiredFeatures{};

    VkDeviceCreateInfo deviceCreateInfo = vku::InitStructHelper{};
    deviceCreateInfo.pNext = requiredFeatures.getChain();
    deviceCreateInfo.flags = 0;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.enabledLayerCount = 0;
    deviceCreateInfo.ppEnabledLayerNames = nullptr;
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensionsStr.size());
    deviceCreateInfo.ppEnabledExtensionNames = extensionsStr.data();
    deviceCreateInfo.pEnabledFeatures = nullptr;

    VkDevice device{VK_NULL_HANDLE};
    if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device) != VK_SUCCESS) {
        return std::unexpected{"failed to create logical device"};
    }

    return device;
}

}  // namespace graphics::vk::impl
