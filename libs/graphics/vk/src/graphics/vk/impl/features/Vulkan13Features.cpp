#include <fmt/core.h>

#include <graphics/vk/impl/features/Vulkan13Features.hpp>
#include <unordered_map>
#include <vulkan/utility/vk_struct_helper.hpp>

namespace {

auto featureNameToPtr() noexcept -> std::unordered_map<std::string, VkBool32 VkPhysicalDeviceVulkan13Features::*> {
    std::unordered_map<std::string, VkBool32 VkPhysicalDeviceVulkan13Features::*> const nameToPtr{
        // {"maintenance4", &VkPhysicalDeviceVulkan13Features::maintenance4},  //
        {"dynamicRendering", &VkPhysicalDeviceVulkan13Features::dynamicRendering},  //
        // {"synchronization2", &VkPhysicalDeviceVulkan13Features::synchronization2},  //
    };

    return nameToPtr;
}

}  // namespace

namespace graphics::vk::impl::features {

Vulkan13Features::Vulkan13Features() noexcept {
    m_features.sType = vku::GetSType<VkPhysicalDeviceVulkan13Features>();

    for (auto const& p : featureNameToPtr()) {
        m_features.*(p.second) = VK_TRUE;
    }
}

auto Vulkan13Features::addToChain(void** pNext) noexcept -> void** {
    *pNext = &m_features;

    return &m_features.pNext;
}

auto Vulkan13Features::check(VkPhysicalDevice physicalDevice) const noexcept -> bool {
    VkPhysicalDeviceVulkan13Features features{};
    features.sType = vku::GetSType<VkPhysicalDeviceVulkan13Features>();

    VkPhysicalDeviceFeatures2 features2{};
    features2.sType = vku::GetSType<VkPhysicalDeviceFeatures2>();
    features2.pNext = &features;

    vkGetPhysicalDeviceFeatures2(physicalDevice, &features2);

    bool result{true};

    for (auto const& p : featureNameToPtr()) {
        if (features.*(p.second) == VK_FALSE) {
            result = false;
            fmt::println("\tVkPhysicalDeviceVulkan13Features::{} is not supported", p.first);
        }
    }

    return result;
}

auto Vulkan13Features::print() const noexcept -> void {
    fmt::println("required VkPhysicalDeviceVulkan13Features:");

    for (auto const& p : featureNameToPtr()) {
        fmt::println("\tVkPhysicalDeviceVulkan13Features::{}", p.first);
    }
}

}  // namespace graphics::vk::impl::features
