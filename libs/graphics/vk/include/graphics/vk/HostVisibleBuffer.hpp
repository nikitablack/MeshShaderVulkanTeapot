#pragma once

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include <expected>
#include <span>
#include <string>

namespace graphics::vk {

class HostVisibleBuffer {
public:
    HostVisibleBuffer() = default;

    HostVisibleBuffer(HostVisibleBuffer const&) = delete;
    auto operator=(HostVisibleBuffer const&) noexcept -> HostVisibleBuffer& = delete;

    HostVisibleBuffer(HostVisibleBuffer&& other) noexcept;
    auto operator=(HostVisibleBuffer&& other) noexcept -> HostVisibleBuffer&;

public:
    [[nodiscard]] auto init(VmaAllocator allocator,  //
                            size_t size,  //
                            VkBufferUsageFlags2 usageFlags = VK_BUFFER_USAGE_2_TRANSFER_DST_BIT |  //
                                                             VK_BUFFER_USAGE_2_TRANSFER_SRC_BIT  //
                            ) noexcept -> std::expected<void, std::string>;

    auto destroy() noexcept -> void;

    [[nodiscard]] auto copyToBuffer(std::span<std::byte const> src,  //
                                    size_t dstOffset = 0  //
                                    ) noexcept -> std::expected<void, std::string>;

    template <typename T>
    [[nodiscard]] auto copyToBuffer(std::span<T const> src,  //
                                    size_t dstOffset = 0  //
                                    ) noexcept -> std::expected<void, std::string> {
        return copyToBuffer(std::as_bytes(src), dstOffset);
    }

    [[nodiscard]] auto copyFromBuffer(std::span<std::byte> dst,  //
                                      size_t srcOffset  //
                                      ) noexcept -> std::expected<void, std::string>;

    template <typename T>
    [[nodiscard]] auto copyFromBuffer(std::span<T> dst,  //
                                      size_t srcOffset  //
                                      ) noexcept -> std::expected<void, std::string> {
        return copyFromBuffer(std::as_writable_bytes(dst), srcOffset);
    }

    auto size() const noexcept -> size_t;
    auto buffer() const noexcept -> VkBuffer;

private:
    VmaAllocator m_allocator{VK_NULL_HANDLE};
    VkBuffer m_buffer{VK_NULL_HANDLE};
    size_t m_size{0};
    VmaAllocation m_allocation{VK_NULL_HANDLE};
};

}  // namespace graphics::vk
