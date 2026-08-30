#include <graphics/vk/Buffer.hpp>
#include <utils/try_expected.hpp>
#include <vulkan/utility/vk_struct_helper.hpp>

namespace graphics::vk {

Buffer::Buffer(Buffer&& other) noexcept
    : m_allocator{other.m_allocator},
      m_device{other.m_device},
      m_buffer{other.m_buffer},
      m_size{other.m_size},
      m_allocation{other.m_allocation} {
    other.m_allocator = VK_NULL_HANDLE;
    other.m_device = VK_NULL_HANDLE;
    other.m_buffer = VK_NULL_HANDLE;
    other.m_size = 0;
    other.m_allocation = VK_NULL_HANDLE;
}

auto Buffer::operator=(Buffer&& other) noexcept -> Buffer& {
    m_allocator = other.m_allocator;
    m_device = other.m_device;
    m_buffer = other.m_buffer;
    m_size = other.m_size;
    m_allocation = other.m_allocation;

    other.m_allocator = VK_NULL_HANDLE;
    other.m_device = VK_NULL_HANDLE;
    other.m_buffer = VK_NULL_HANDLE;
    other.m_size = 0;
    other.m_allocation = VK_NULL_HANDLE;

    return *this;
}

auto Buffer::init(VmaAllocator allocator,  //
                  VkDevice device,  //
                  size_t size,  //
                  VkBufferUsageFlags2 usageFlags  //
                  ) noexcept -> std::expected<void, std::string> {
    m_allocator = allocator;
    m_device = device;
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
    allocationCreateInfo.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
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
        return std::unexpected{"failed to create buffer"};
    }

    return {};
}

auto Buffer::destroy() noexcept -> void {
    if (!m_buffer) {
        return;
    }

    vmaDestroyBuffer(m_allocator, m_buffer, m_allocation);

    m_allocator = VK_NULL_HANDLE;
    m_device = VK_NULL_HANDLE;
    m_buffer = VK_NULL_HANDLE;
    m_size = 0;
    m_allocation = VK_NULL_HANDLE;
}

auto Buffer::size() const noexcept -> size_t {
    return m_size;
}

auto Buffer::buffer() const noexcept -> VkBuffer {
    return m_buffer;
}

}  // namespace graphics::vk
