#include <graphics/vk/FrameData.hpp>
#include <graphics/vk/Image2d.hpp>
#include <graphics/vk/impl/draw_fullscreen_triangle.hpp>
#include <graphics/vk/utils/get_push_constant_data.hpp>
#include <vulkan/utility/vk_struct_helper.hpp>

namespace graphics::vk::impl {

auto draw_fullscreen_triangle(VkDevice device,  //
                              VkPipeline pipeline,  //
                              VkPipelineLayout pipelineLayout,  //
                              VkSampler renderTargetSampler,  //
                              Image2d const& renderTarget,  //
                              VkExtent2D const& surfaceExtent,  //
                              FrameData& frameData  //
                              ) noexcept -> void {
    VkRect2D scissor{};
    scissor.offset = VkOffset2D{0, 0};
    scissor.extent = surfaceExtent;

    vkCmdSetScissor(frameData.commandBuffer, 0, 1, &scissor);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(surfaceExtent.width);
    viewport.height = static_cast<float>(surfaceExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    vkCmdSetViewport(frameData.commandBuffer, 0, 1, &viewport);

    VkDescriptorImageInfo imageInfo{};
    imageInfo.sampler = renderTargetSampler;
    imageInfo.imageView = renderTarget.view();
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkWriteDescriptorSet writeDescriptorSet = vku::InitStructHelper{};
    writeDescriptorSet.dstSet = frameData.cisDescriptorSet;
    writeDescriptorSet.dstBinding = 0;
    writeDescriptorSet.dstArrayElement = frameData.cisDescriptorCounter++;
    writeDescriptorSet.descriptorCount = 1;
    writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writeDescriptorSet.pImageInfo = &imageInfo;
    writeDescriptorSet.pBufferInfo = nullptr;
    writeDescriptorSet.pTexelBufferView = nullptr;

    vkUpdateDescriptorSets(device, 1, &writeDescriptorSet, 0, nullptr);

    auto const pushConstData{utils::get_push_constant_data(writeDescriptorSet.dstArrayElement)};

    vkCmdPushConstants(frameData.commandBuffer,  //
                       pipelineLayout,  //
                       VK_SHADER_STAGE_ALL,  //
                       0,  //
                       static_cast<uint32_t>(pushConstData.size()),  //
                       pushConstData.data());

    vkCmdBindPipeline(frameData.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

    vkCmdDraw(frameData.commandBuffer, 3, 1, 0, 0);
}

}  // namespace graphics::vk::impl
