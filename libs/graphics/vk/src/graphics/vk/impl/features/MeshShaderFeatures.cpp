#include <fmt/core.h>

#include <graphics/vk/impl/features/MeshShaderFeatures.hpp>
#include <unordered_map>
#include <vulkan/utility/vk_struct_helper.hpp>

namespace {

auto featureNameToPtr() noexcept -> std::unordered_map<std::string, VkBool32 VkPhysicalDeviceMeshShaderFeaturesEXT::*> {
    std::unordered_map<std::string, VkBool32 VkPhysicalDeviceMeshShaderFeaturesEXT::*> const nameToPtr{
        {"taskShader", &VkPhysicalDeviceMeshShaderFeaturesEXT::taskShader},  //
        {"meshShader", &VkPhysicalDeviceMeshShaderFeaturesEXT::meshShader},  //
    };

    return nameToPtr;
}

}  // namespace

namespace graphics::vk::impl::features {

MeshShaderFeatures::MeshShaderFeatures() noexcept {
    m_features.sType = vku::GetSType<VkPhysicalDeviceMeshShaderFeaturesEXT>();

    for (auto const& p : featureNameToPtr()) {
        m_features.*(p.second) = VK_TRUE;
    }
}

auto MeshShaderFeatures::addToChain(void** pNext) noexcept -> void** {
    *pNext = &m_features;

    return &m_features.pNext;
}

auto MeshShaderFeatures::check(VkPhysicalDevice physicalDevice) const noexcept -> bool {
    VkPhysicalDeviceMeshShaderFeaturesEXT features{};
    features.sType = vku::GetSType<VkPhysicalDeviceMeshShaderFeaturesEXT>();

    VkPhysicalDeviceFeatures2 features2{};
    features2.sType = vku::GetSType<VkPhysicalDeviceFeatures2>();
    features2.pNext = &features;

    vkGetPhysicalDeviceFeatures2(physicalDevice, &features2);

    bool result{true};

    for (auto const& p : featureNameToPtr()) {
        if (features.*(p.second) == VK_FALSE) {
            result = false;
            fmt::println("\tVkPhysicalDeviceMeshShaderFeaturesEXT::{} is not supported", p.first);
        }
    }

    return result;
}

auto MeshShaderFeatures::print() const noexcept -> void {
    fmt::println("required VkPhysicalDeviceMeshShaderFeaturesEXT:");

    for (auto const& p : featureNameToPtr()) {
        fmt::println("\tVkPhysicalDeviceMeshShaderFeaturesEXT::{}", p.first);
    }
}

}  // namespace graphics::vk::impl::features
