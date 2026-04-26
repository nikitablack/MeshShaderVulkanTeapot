#include <fmt/core.h>

#include <graphics/vk/impl/create_swapchain_image_views.hpp>
#include <vulkan/utility/vk_struct_helper.hpp>

namespace graphics::vk::impl {

auto create_swaphain_image_views(VkDevice device,  //
                                 std::vector<VkImage> const& images,  //
                                 VkFormat surfaceFormat  //
                                 ) noexcept -> std::expected<std::vector<VkImageView>, std::string> {
    fmt::println("creating swapchain image views");

    std::vector<VkImageView> views{};
    views.reserve(images.size());

    VkComponentMapping mapping{};
    mapping.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    mapping.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    mapping.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    mapping.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    VkImageSubresourceRange srRange{};
    srRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    srRange.baseMipLevel = 0;
    srRange.levelCount = 1;
    srRange.baseArrayLayer = 0;
    srRange.layerCount = 1;

    VkImageViewCreateInfo info{};
    info.sType = vku::GetSType<VkImageViewCreateInfo>();
    info.pNext = nullptr;
    info.flags = 0;
    info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    info.format = surfaceFormat;
    info.components = mapping;
    info.subresourceRange = srRange;

    for (size_t i{0}; i < images.size(); ++i) {
        info.image = images[i];

        VkImageView view{VK_NULL_HANDLE};
        if (vkCreateImageView(device, &info, nullptr, &view) != VK_SUCCESS) {
            return std::unexpected{fmt::format("failed to create image view {}", i)};
        }

        views.push_back(view);
    }

    return views;
}

}  // namespace graphics::vk::impl