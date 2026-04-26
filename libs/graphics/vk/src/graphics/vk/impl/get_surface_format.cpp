#include <fmt/core.h>

#include <graphics/vk/impl/get_surface_format.hpp>
#include <vector>
#include <vulkan/utility/vk_struct_helper.hpp>

namespace graphics::vk::impl {

auto get_surface_format(VkPhysicalDevice physicalDevice,  //
                        VkSurfaceKHR surface  //
                        ) noexcept -> std::expected<VkSurfaceFormat2KHR, std::string> {
    fmt::println("getting surface format");

    uint32_t formatCount{0};

    VkPhysicalDeviceSurfaceInfo2KHR surfaceInfo{};
    surfaceInfo.sType = vku::GetSType<VkPhysicalDeviceSurfaceInfo2KHR>();
    surfaceInfo.pNext = nullptr;
    surfaceInfo.surface = surface;

    if ((vkGetPhysicalDeviceSurfaceFormats2KHR(physicalDevice, &surfaceInfo, &formatCount, nullptr) != VK_SUCCESS) ||
        (formatCount == 0)) {
        return std::unexpected{"failed to get physical device surface formats"};
    }

    std::vector<VkSurfaceFormat2KHR> formats(formatCount);
    for (size_t i{0}; i < formats.size(); ++i) {
        formats[i].sType = vku::GetSType<VkSurfaceFormat2KHR>();
    }

    if (vkGetPhysicalDeviceSurfaceFormats2KHR(physicalDevice, &surfaceInfo, &formatCount, formats.data()) !=
        VK_SUCCESS) {
        return std::unexpected{"failed to get physical device surface formats"};
    }

    for (auto const& format : formats) {
        if ((format.surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) &&
            ((format.surfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB) ||
             (format.surfaceFormat.format == VK_FORMAT_R8G8B8A8_SRGB))) {
            return format;
        }
    }

    return formats[0];
}

}  // namespace graphics::vk::impl
