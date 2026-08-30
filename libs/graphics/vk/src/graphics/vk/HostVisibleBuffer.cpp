#include <graphics/vk/HostVisibleBuffer.hpp>
#include <utils/try_expected.hpp>
#include <vulkan/utility/vk_struct_helper.hpp>

namespace graphics::vk {

HostVisibleBuffer::HostVisibleBuffer(HostVisibleBuffer&& other) noexcept
    : m_allocator{other.m_allocator}, m_buffer{other.m_buffer}, m_size{other.m_size}, m_allocation{other.m_allocation} {
    other.m_allocator = VK_NULL_HANDLE;
    other.m_buffer = VK_NULL_HANDLE;
    other.m_size = 0;
    other.m_allocation = VK_NULL_HANDLE;
}

auto HostVisibleBuffer::operator=(HostVisibleBuffer&& other) noexcept -> HostVisibleBuffer& {
    m_allocator = other.m_allocator;
    m_buffer = other.m_buffer;
    m_size = other.m_size;
    m_allocation = other.m_allocation;

    other.m_allocator = VK_NULL_HANDLE;
    other.m_buffer = VK_NULL_HANDLE;
    other.m_size = 0;
    other.m_allocation = VK_NULL_HANDLE;

    return *this;
}

auto HostVisibleBuffer::init(VmaAllocator allocator,  //
                             size_t size,  //
                             VkBufferUsageFlags2 usageFlags  //
                             ) noexcept -> std::expected<void, std::string> {
    m_allocator = allocator;
    m_size = size;

    VkBufferUsageFlags2CreateInfoKHR flagsCreateInfo = vku::InitStructHelper{};
    flagsCreateInfo.usage = usageFlags;

    VkBufferCreateInfo bufferCreateInfo = vku::InitStructHelper{&flagsCreateInfo};
    bufferCreateInfo.flags = 0;
    bufferCreateInfo.size = m_size;
    bufferCreateInfo.usage = 0;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferCreateInfo.queueFamilyIndexCount = 0;
    bufferCreateInfo.pQueueFamilyIndices = nullptr;

    VmaAllocationCreateInfo allocationCreateInfo{};
    allocationCreateInfo.flags =
        VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;
    allocationCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
    allocationCreateInfo.requiredFlags = 0;
    allocationCreateInfo.preferredFlags = 0;
    allocationCreateInfo.memoryTypeBits = 0;
    allocationCreateInfo.pool = VK_NULL_HANDLE;
    allocationCreateInfo.pUserData = nullptr;
    allocationCreateInfo.priority = 0.0f;

    if (vmaCreateBuffer(m_allocator,  //
                        &bufferCreateInfo,  //
                        &allocationCreateInfo,  //
                        &m_buffer,  //
                        &m_allocation,  //
                        nullptr) != VK_SUCCESS) {
        return std::unexpected{"failed to create host-visible buffer"};
    }

    return {};
}

auto HostVisibleBuffer::destroy() noexcept -> void {
    if (!m_buffer) {
        return;
    }

    vmaDestroyBuffer(m_allocator, m_buffer, m_allocation);

    m_allocator = VK_NULL_HANDLE;
    m_buffer = VK_NULL_HANDLE;
    m_size = 0;
    m_allocation = VK_NULL_HANDLE;
}

auto HostVisibleBuffer::copyToBuffer(std::span<std::byte const> src,  //
                                     size_t dstOffset  //
                                     ) noexcept -> std::expected<void, std::string> {
    if (!m_allocation) {
        return std::unexpected{"HostVisibleBuffer is not initialized. Did you forget to call init()?"};
    }

    if (m_size < (dstOffset + src.size_bytes())) {
        return std::unexpected{"HostVisibleBuffer is too small"};
    }

    if (vmaCopyMemoryToAllocation(m_allocator,  //
                                  src.data(),  //
                                  m_allocation,  //
                                  dstOffset,  //
                                  src.size_bytes()) != VK_SUCCESS) {
        return std::unexpected{"failed to copy data to host-visible buffer"};
    }

    return {};
}

auto HostVisibleBuffer::copyFromBuffer(std::span<std::byte> dst,  //
                                       size_t srcOffset  //
                                       ) noexcept -> std::expected<void, std::string> {
    if (!m_allocation) {
        return std::unexpected{"HostVisibleBuffer is not initialized. Did you forget to call init()?"};
    }

    if (m_size < (srcOffset + dst.size_bytes())) {
        return std::unexpected{"HostVisibleBuffer is too small"};
    }

    if (vmaCopyAllocationToMemory(m_allocator,  //
                                  m_allocation,  //
                                  srcOffset,  //
                                  dst.data(),  //
                                  dst.size_bytes()) != VK_SUCCESS) {
        return std::unexpected{"failed to copy data from host-visible buffer"};
    }

    return {};
}

auto HostVisibleBuffer::size() const noexcept -> size_t {
    return m_size;
}

auto HostVisibleBuffer::buffer() const noexcept -> VkBuffer {
    return m_buffer;
}

}  // namespace graphics::vk
