#pragma once

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include <expected>
#include <string>

namespace graphics::vk {

class Image2d {
public:
    Image2d() = default;

public:
    [[nodiscard]] auto init(VmaAllocator allocator,  //
                            VkDevice device,  //
                            VkFormat format,  //
                            VkImageUsageFlags usageFlags,  //
                            VkImageAspectFlags aspectFlags,  //
                            uint32_t width,  //
                            uint32_t height  //
                            ) noexcept -> std::expected<void, std::string>;

    auto destroy() noexcept -> void;

    auto width() const noexcept -> uint32_t;
    auto height() const noexcept -> uint32_t;
    auto size() const noexcept -> size_t;
    auto image() const noexcept -> VkImage;
    auto view() const noexcept -> VkImageView;
    auto aspectFlags() const noexcept -> VkImageAspectFlags;

private:
    VmaAllocator m_allocator{VK_NULL_HANDLE};
    VkDevice m_device{VK_NULL_HANDLE};
    VkImage m_image{VK_NULL_HANDLE};
    VkImageView m_view{VK_NULL_HANDLE};
    uint32_t m_width{0};
    uint32_t m_height{0};
    size_t m_size{0};
    VmaAllocation m_allocation{VK_NULL_HANDLE};
    VkImageAspectFlags m_aspectFlags{0};
};

}  // namespace graphics::vk
