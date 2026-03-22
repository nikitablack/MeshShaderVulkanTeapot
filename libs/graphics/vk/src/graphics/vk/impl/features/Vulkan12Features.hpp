#pragma once

#include <graphics/vk/impl/features/Features.hpp>

namespace graphics::vk::impl::features {

class Vulkan12Features : public Features {
public:
    Vulkan12Features() noexcept;

public:
    virtual auto addToChain(void** pNext) noexcept -> void** override;
    virtual auto check(VkPhysicalDevice physicalDevice) const noexcept -> bool override;
    virtual auto print() const noexcept -> void override;

private:
    VkPhysicalDeviceVulkan12Features m_features{};
};

static_assert(DerivedFromFeatures<Vulkan12Features>);

}  // namespace graphics::vk::impl::features
