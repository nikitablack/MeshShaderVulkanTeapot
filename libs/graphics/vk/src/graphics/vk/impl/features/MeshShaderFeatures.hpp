#pragma once

#include <graphics/vk/impl/features/Features.hpp>

namespace graphics::vk::impl::features {

class MeshShaderFeatures : public Features {
public:
    MeshShaderFeatures() noexcept;

public:
    virtual auto addToChain(void** pNext) noexcept -> void** override;
    virtual auto check(VkPhysicalDevice physicalDevice) const noexcept -> bool override;
    virtual auto print() const noexcept -> void override;

private:
    VkPhysicalDeviceMeshShaderFeaturesEXT m_features{};
};

static_assert(DerivedFromFeatures<MeshShaderFeatures>);

}  // namespace graphics::vk::impl::features
