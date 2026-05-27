#include <graphics/vk/impl/get_physical_device_properties.hpp>
#include <vulkan/utility/vk_struct_helper.hpp>

namespace graphics::vk::impl {

[[nodiscard]] auto get_physical_device_properties(VkPhysicalDevice device) noexcept -> VkPhysicalDeviceProperties2 {
    VkPhysicalDeviceProperties2 properties = vku::InitStructHelper{};

    vkGetPhysicalDeviceProperties2(device, &properties);

    return properties;
}

}  // namespace graphics::vk::impl