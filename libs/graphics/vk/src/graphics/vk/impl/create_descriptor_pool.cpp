#include <fmt/core.h>

#include <array>
#include <graphics/vk/impl/create_descriptor_pool.hpp>
#include <vulkan/utility/vk_struct_helper.hpp>

namespace graphics::vk::impl {

auto create_descriptor_pool(VkDevice device,  //
                            uint32_t descriptorCount  //
                            ) noexcept -> std::expected<VkDescriptorPool, std::string> {
    fmt::println("creating descriptor pool");

    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    poolSizes[0].descriptorCount = descriptorCount;
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = descriptorCount;

    VkDescriptorPoolCreateInfo info = vku::InitStructHelper{};
    info.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT;
    info.maxSets = 2;
    info.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    info.pPoolSizes = poolSizes.data();

    VkDescriptorPool pool{VK_NULL_HANDLE};
    if (vkCreateDescriptorPool(device, &info, nullptr, &pool) != VK_SUCCESS) {
        return std::unexpected{"failed to create descriptor pool"};
    }

    return pool;
}

}  // namespace graphics::vk::impl
