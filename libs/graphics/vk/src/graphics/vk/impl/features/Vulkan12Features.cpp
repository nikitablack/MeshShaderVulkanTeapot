#include <fmt/core.h>

#include <graphics/vk/impl/features/Vulkan12Features.hpp>
#include <unordered_map>
#include <vulkan/utility/vk_struct_helper.hpp>

namespace {

auto featureNameToPtr() noexcept -> std::unordered_map<std::string, VkBool32 VkPhysicalDeviceVulkan12Features::*> {
    std::unordered_map<std::string, VkBool32 VkPhysicalDeviceVulkan12Features::*> const nameToPtr{
        // {"descriptorBindingPartiallyBound", &VkPhysicalDeviceVulkan12Features::descriptorBindingPartiallyBound},  //
        // {"descriptorBindingStorageBufferUpdateAfterBind",
        //  &VkPhysicalDeviceVulkan12Features::descriptorBindingStorageBufferUpdateAfterBind},  //
        // {"descriptorBindingStorageImageUpdateAfterBind",
        //  &VkPhysicalDeviceVulkan12Features::descriptorBindingStorageImageUpdateAfterBind},  //
        // {"descriptorBindingVariableDescriptorCount",
        //  &VkPhysicalDeviceVulkan12Features::descriptorBindingVariableDescriptorCount},  //
        // {"shaderStorageBufferArrayNonUniformIndexing",
        //  &VkPhysicalDeviceVulkan12Features::shaderStorageBufferArrayNonUniformIndexing},  //
        // {"shaderSampledImageArrayNonUniformIndexing",
        //  &VkPhysicalDeviceVulkan12Features::shaderSampledImageArrayNonUniformIndexing},  //
        // {"runtimeDescriptorArray", &VkPhysicalDeviceVulkan12Features::runtimeDescriptorArray},  //
        {"scalarBlockLayout", &VkPhysicalDeviceVulkan12Features::scalarBlockLayout},  //
    };

    return nameToPtr;
}

}  // namespace

namespace graphics::vk::impl::features {

Vulkan12Features::Vulkan12Features() noexcept {
    m_features = vku::InitStructHelper{};

    for (auto const& p : featureNameToPtr()) {
        m_features.*(p.second) = VK_TRUE;
    }
}

auto Vulkan12Features::addToChain(void** pNext) noexcept -> void** {
    *pNext = &m_features;

    return &m_features.pNext;
}

auto Vulkan12Features::check(VkPhysicalDevice physicalDevice) const noexcept -> bool {
    VkPhysicalDeviceVulkan12Features features = vku::InitStructHelper{};
    VkPhysicalDeviceFeatures2 features2 = vku::InitStructHelper{&features};

    vkGetPhysicalDeviceFeatures2(physicalDevice, &features2);

    bool result{true};

    for (auto const& p : featureNameToPtr()) {
        if (features.*(p.second) == VK_FALSE) {
            result = false;
            fmt::println("\tVkPhysicalDeviceVulkan12Features::{} is not supported", p.first);
        }
    }

    return result;
}

auto Vulkan12Features::print() const noexcept -> void {
    fmt::println("required VkPhysicalDeviceVulkan12Features:");

    for (auto const& p : featureNameToPtr()) {
        fmt::println("\tVkPhysicalDeviceVulkan12Features::{}", p.first);
    }
}

}  // namespace graphics::vk::impl::features
