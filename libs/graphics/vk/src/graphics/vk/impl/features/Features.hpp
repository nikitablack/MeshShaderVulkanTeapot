#pragma once

#include <vulkan/vulkan.h>

#include <type_traits>

namespace graphics::vk::impl::features {

class Features {
public:
    virtual ~Features() = default;

public:
    virtual auto addToChain(void** pNext) noexcept -> void** = 0;
    virtual auto check(VkPhysicalDevice physicalDevice) const noexcept -> bool = 0;
    virtual auto print() const noexcept -> void = 0;
};

template <typename T>
concept DerivedFromFeatures = std::is_base_of_v<Features, T>;

}  // namespace graphics::vk::impl::features
