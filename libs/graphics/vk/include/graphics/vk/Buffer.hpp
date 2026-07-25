#pragma once

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include <expected>
#include <string>

namespace graphics::vk {

class Buffer {
public:
    Buffer() = default;

public:
    [[nodiscard]] auto init(VmaAllocator allocator,  //
                            VkDevice device,  //
                            size_t size,  //
                            VkBufferUsageFlags2 usageFlags = VK_BUFFER_USAGE_2_STORAGE_BUFFER_BIT |  //
                                                             VK_BUFFER_USAGE_2_TRANSFER_DST_BIT |  //
                                                             VK_BUFFER_USAGE_2_TRANSFER_SRC_BIT  //
                            ) noexcept -> std::expected<void, std::string>;

    auto destroy() noexcept -> void;

    auto size() const noexcept -> size_t;
    auto buffer() const noexcept -> VkBuffer;

private:
    VmaAllocator m_allocator{VK_NULL_HANDLE};
    VkDevice m_device{VK_NULL_HANDLE};
    VkBuffer m_buffer{VK_NULL_HANDLE};
    size_t m_size{0};
    VmaAllocation m_allocation{VK_NULL_HANDLE};
};

}  // namespace graphics::vk
