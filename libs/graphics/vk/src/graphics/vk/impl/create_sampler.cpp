#include <fmt/core.h>

#include <graphics/vk/impl/create_sampler.hpp>
#include <vulkan/utility/vk_struct_helper.hpp>

namespace graphics::vk::impl {

auto create_sampler(VkDevice device) noexcept -> std::expected<VkSampler, std::string> {
    fmt::println("creating sampler");

    VkSamplerCreateInfo info = vku::InitStructHelper{};
    info.flags = 0;
    info.magFilter = VK_FILTER_LINEAR;
    info.minFilter = VK_FILTER_LINEAR;
    info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    info.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    info.mipLodBias = 0.0f;
    info.anisotropyEnable = VK_FALSE;
    info.maxAnisotropy = 0.0f;
    info.compareEnable = VK_FALSE;
    info.compareOp = VK_COMPARE_OP_NEVER;
    info.minLod = 0.0f;
    info.maxLod = 0.0f;
    info.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
    info.unnormalizedCoordinates = VK_FALSE;

    VkSampler sampler{VK_NULL_HANDLE};
    if (vkCreateSampler(device, &info, nullptr, &sampler) != VK_SUCCESS) {
        return std::unexpected{"failed to create sampler"};
    }

    return sampler;
}

}  // namespace graphics::vk::impl
