#include <fmt/core.h>

#include <array>
#include <graphics/vk/impl/create_pipeline_layout.hpp>
#include <vulkan/utility/vk_struct_helper.hpp>

namespace graphics::vk::impl {

auto create_pipeline_layout(VkDevice device,  //
                            VkDescriptorSetLayout storageDescriptorSetLayout,  //
                            VkDescriptorSetLayout cisDescriptorSetLayout  //
                            ) noexcept -> std::expected<VkPipelineLayout, std::string> {
    fmt::println("creating pipeline layout");

    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_ALL;
    pushConstantRange.offset = 0;
    pushConstantRange.size = 128;

    std::array<VkDescriptorSetLayout, 2> setLayouts{storageDescriptorSetLayout, cisDescriptorSetLayout};

    VkPipelineLayoutCreateInfo info = vku::InitStructHelper{};
    info.flags = 0;
    info.setLayoutCount = static_cast<uint32_t>(setLayouts.size());
    info.pSetLayouts = setLayouts.data();
    info.pushConstantRangeCount = 1;
    info.pPushConstantRanges = &pushConstantRange;

    VkPipelineLayout pipelineLayout{VK_NULL_HANDLE};
    if (vkCreatePipelineLayout(device, &info, nullptr, &pipelineLayout) != VK_SUCCESS) {
        return std::unexpected{"failed to create pipeline layout"};
    }

    return pipelineLayout;
}

}  // namespace graphics::vk::impl
