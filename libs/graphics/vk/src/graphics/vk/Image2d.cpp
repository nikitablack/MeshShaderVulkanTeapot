#include <vulkan/utility/vk_format_utils.h>

#include <graphics/vk/Image2d.hpp>
#include <vulkan/utility/vk_struct_helper.hpp>

namespace graphics::vk {

auto Image2d::init(VmaAllocator allocator,  //
                   VkDevice device,  //
                   VkFormat format,  //
                   VkImageUsageFlags usageFlags,  //
                   VkImageAspectFlags aspectFlags,  //
                   uint32_t width,  //
                   uint32_t height  //
                   ) noexcept -> std::expected<void, std::string> {
    VkExtent3D const blockExtent{vkuFormatTexelBlockExtent(format)};

    uint32_t const blocksX{(width + blockExtent.width - 1) / blockExtent.width};
    uint32_t const blocksY{(height + blockExtent.height - 1) / blockExtent.height};
    uint32_t const blocksZ{1};

    uint32_t blockSize{vkuFormatTexelBlockSize(format)};

    m_size = static_cast<size_t>(blocksX * blocksY * blocksZ * blockSize);

    m_allocator = allocator;
    m_device = device;
    m_width = width;
    m_height = height;
    m_aspectFlags = aspectFlags;

    VkExtent3D extent{};
    extent.width = m_width;
    extent.height = m_height;
    extent.depth = 1;

    VkImageCreateInfo imageCreateInfo = vku::InitStructHelper{};
    imageCreateInfo.flags = 0;
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.format = format;
    imageCreateInfo.extent = extent;
    imageCreateInfo.mipLevels = 1;
    imageCreateInfo.arrayLayers = 1;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageCreateInfo.usage = usageFlags;
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageCreateInfo.queueFamilyIndexCount = 0;
    imageCreateInfo.pQueueFamilyIndices = nullptr;
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    VmaAllocationCreateInfo allocationCreateInfo{};
    allocationCreateInfo.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
    allocationCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
    allocationCreateInfo.requiredFlags = 0;
    allocationCreateInfo.preferredFlags = 0;
    allocationCreateInfo.memoryTypeBits = 0;
    allocationCreateInfo.pool = VK_NULL_HANDLE;
    allocationCreateInfo.pUserData = nullptr;
    allocationCreateInfo.priority = 0.0f;

    if (vmaCreateImage(m_allocator, &imageCreateInfo, &allocationCreateInfo, &m_image, &m_allocation, nullptr) !=
        VK_SUCCESS) {
        return std::unexpected{"failed to create image"};
    }

    VkImageSubresourceRange subresourceRange{};
    subresourceRange.aspectMask = m_aspectFlags;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = 1;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = 1;

    VkImageViewCreateInfo viewCreateInfo = vku::InitStructHelper{};
    viewCreateInfo.flags = 0;
    viewCreateInfo.image = m_image;
    viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewCreateInfo.format = format;
    viewCreateInfo.components = VkComponentMapping{VK_COMPONENT_SWIZZLE_IDENTITY,  //
                                                   VK_COMPONENT_SWIZZLE_IDENTITY,  //
                                                   VK_COMPONENT_SWIZZLE_IDENTITY,  //
                                                   VK_COMPONENT_SWIZZLE_IDENTITY};
    viewCreateInfo.subresourceRange = subresourceRange;

    if (vkCreateImageView(m_device, &viewCreateInfo, nullptr, &m_view) != VK_SUCCESS) {
        return std::unexpected{"failed to create image view"};
    }

    return {};
}

auto Image2d::destroy() noexcept -> void {
    if (!m_allocator || !m_device) {
        return;
    }

    vmaDestroyImage(m_allocator, m_image, m_allocation);
    m_image = VK_NULL_HANDLE;

    vkDestroyImageView(m_device, m_view, nullptr);
    m_view = VK_NULL_HANDLE;

    m_allocator = VK_NULL_HANDLE;
    m_device = VK_NULL_HANDLE;
    m_allocation = VK_NULL_HANDLE;
    m_width = 0;
    m_height = 0;
    m_size = 0;
    m_aspectFlags = 0;
}

auto Image2d::width() const noexcept -> uint32_t {
    return m_width;
}

auto Image2d::height() const noexcept -> uint32_t {
    return m_height;
}

auto Image2d::size() const noexcept -> size_t {
    return m_size;
}

auto Image2d::image() const noexcept -> VkImage {
    return m_image;
}

auto Image2d::view() const noexcept -> VkImageView {
    return m_view;
}

auto Image2d::aspectFlags() const noexcept -> VkImageAspectFlags {
    return m_aspectFlags;
}

}  // namespace graphics::vk
