#include <fmt/core.h>

#include <graphics/vk/impl/get_surface_capabilities.hpp>
#include <vulkan/utility/vk_struct_helper.hpp>

namespace graphics::vk::impl {

[[nodiscard]] auto get_surface_capabilities(VkPhysicalDevice physicalDevice,  //
                                            VkSurfaceKHR surface  //
                                            ) noexcept -> std::expected<VkSurfaceCapabilities2KHR, std::string> {
    fmt::println("getting surface capabilities");

    VkPhysicalDeviceSurfaceInfo2KHR info = vku::InitStructHelper{};
    info.surface = surface;

    VkSurfaceCapabilities2KHR caps = vku::InitStructHelper{};

    if (vkGetPhysicalDeviceSurfaceCapabilities2KHR(physicalDevice, &info, &caps) != VK_SUCCESS) {
        return std::unexpected{"failed to get surface capabilities"};
    }

    return caps;
}

}  // namespace graphics::vk::impl
