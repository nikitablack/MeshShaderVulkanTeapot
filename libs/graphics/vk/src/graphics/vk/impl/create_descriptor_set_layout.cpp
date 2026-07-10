#include <fmt/core.h>

#include <graphics/vk/impl/create_descriptor_set_layout.hpp>
#include <vulkan/utility/vk_struct_helper.hpp>

namespace graphics::vk::impl {

auto create_descriptor_set_layout(VkDevice device,  //
                                  VkDescriptorType descriptorType,  //
                                  uint32_t maxRequiredDescriptorCount  //
                                  ) noexcept -> std::expected<VkDescriptorSetLayout, std::string> {
    fmt::println("creating descriptor set layout");

    VkDescriptorBindingFlags bindingFlags{VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT |  //
                                          VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT |  //
                                          VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT};

    VkDescriptorSetLayoutBindingFlagsCreateInfo flagsInfo = vku::InitStructHelper{};
    flagsInfo.bindingCount = 1;
    flagsInfo.pBindingFlags = &bindingFlags;

    VkDescriptorSetLayoutBinding binding{};
    binding.binding = 0;
    binding.descriptorType = descriptorType;
    binding.descriptorCount = 0;
    binding.stageFlags = VK_SHADER_STAGE_ALL;
    binding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutCreateInfo createInfo = vku::InitStructHelper{&flagsInfo};
    createInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT;
    createInfo.bindingCount = 1;
    createInfo.pBindings = &binding;

    VkDescriptorSetVariableDescriptorCountLayoutSupport variableSupport = vku::InitStructHelper{};
    variableSupport.maxVariableDescriptorCount = 0;

    VkDescriptorSetLayoutSupport support = vku::InitStructHelper{&variableSupport};
    support.supported = VK_FALSE;

    vkGetDescriptorSetLayoutSupport(device, &createInfo, &support);

    if (!support.supported) {
        return std::unexpected{"descriptor set layout is not supported"};
    }

    if (variableSupport.maxVariableDescriptorCount < maxRequiredDescriptorCount) {
        return std::unexpected{
            fmt::format("descriptor set layout max variable descriptor count is too low (max: {}, required: {})",
                        variableSupport.maxVariableDescriptorCount, maxRequiredDescriptorCount)};
    }

    binding.descriptorCount = maxRequiredDescriptorCount;

    VkDescriptorSetLayout layout{VK_NULL_HANDLE};
    if (vkCreateDescriptorSetLayout(device, &createInfo, nullptr, &layout) != VK_SUCCESS) {
        return std::unexpected{"failed to create descriptor set layout"};
    }

    return layout;
}

}  // namespace graphics::vk::impl