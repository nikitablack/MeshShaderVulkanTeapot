#include <imgui.h>

#include <graphics/vk/FrameData.hpp>
#include <graphics/vk/impl/draw_imgui.hpp>
#include <graphics/vk/utils/get_push_constant_data.hpp>
#include <graphics/vk/utils/init_helper.hpp>
#include <optional>
#include <span>
#include <utils/try_expected.hpp>
#include <vulkan/utility/vk_struct_helper.hpp>

namespace {

auto padding(size_t offset, size_t alignment) -> size_t {
    auto misalignment{offset % alignment};
    if (misalignment > 0) {
        return alignment - misalignment;
    }

    return 0;
}

}  // namespace

namespace graphics::vk::impl {

auto draw_imgui(FrameData& frameData,  //
                VkDevice device,  //
                VkPipelineLayout pipelineLayout,  //
                VkPipeline pipeline,  //
                VkExtent2D surfaceExtent,  //
                VkSampler sampler,  //
                VkPhysicalDeviceProperties2 const& deviceProperties  //
                ) noexcept -> std::expected<void, std::string> {
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(surfaceExtent.width);
    viewport.height = static_cast<float>(surfaceExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    vkCmdSetViewport(frameData.commandBuffer, 0, 1, &viewport);

    vkCmdBindPipeline(frameData.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

    auto const* const drawData{ImGui::GetDrawData()};

    [[maybe_unused]] auto const imguiFramePushConstData{
        utils::get_push_constant_data(2.0f / drawData->DisplaySize.x,  //
                                      2.0f / drawData->DisplaySize.y,
                                      -1.0f,  //
                                      -1.0f)};

    vkCmdPushConstants(frameData.commandBuffer,  //
                       pipelineLayout,  //
                       VK_SHADER_STAGE_ALL,  //
                       0,
                       static_cast<uint32_t>(imguiFramePushConstData.size()),  //
                       imguiFramePushConstData.data());

    size_t currImguiBufferOffset{0};

    for (auto const& cmdList : drawData->CmdLists) {
        uint32_t vertexDataBinding{};

        // vertex data
        {
            auto const vertexDataSize{static_cast<size_t>(cmdList->VtxBuffer.size_in_bytes())};
            std::span<std::byte const> vertexData{
                std::as_bytes(std::span{cmdList->VtxBuffer.Data, static_cast<size_t>(cmdList->VtxBuffer.size())})};

            if (frameData.imguiBuffer->size() < (currImguiBufferOffset + vertexDataSize)) {
                return std::unexpected{"imgui buffer is too small"};
            }

            TRY_EXPECTED_VOID(frameData.imguiBuffer->copyToBuffer(vertexData, currImguiBufferOffset));

            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = frameData.imguiBuffer->buffer();
            bufferInfo.offset = currImguiBufferOffset;
            bufferInfo.range = vertexDataSize;

            VkWriteDescriptorSet writeDescriptorSet = vku::InitStructHelper{};
            writeDescriptorSet.dstSet = frameData.storageDescriptorSet;
            writeDescriptorSet.dstBinding = 0;
            writeDescriptorSet.dstArrayElement = frameData.storageDescriptorCounter++;
            writeDescriptorSet.descriptorCount = 1;
            writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            writeDescriptorSet.pImageInfo = nullptr;
            writeDescriptorSet.pBufferInfo = &bufferInfo;
            writeDescriptorSet.pTexelBufferView = nullptr;

            vkUpdateDescriptorSets(device, 1, &writeDescriptorSet, 0, nullptr);

            vertexDataBinding = writeDescriptorSet.dstArrayElement;

            currImguiBufferOffset += vertexDataSize;
            currImguiBufferOffset +=
                padding(currImguiBufferOffset, deviceProperties.properties.limits.minStorageBufferOffsetAlignment);
        }

        // index data
        {
            auto const indexDataSize{static_cast<size_t>(cmdList->IdxBuffer.size_in_bytes())};
            std::span<std::byte const> idxData{
                std::as_bytes(std::span{cmdList->IdxBuffer.Data, static_cast<size_t>(cmdList->IdxBuffer.size())})};

            if (frameData.imguiBuffer->size() < (currImguiBufferOffset + indexDataSize)) {
                return std::unexpected{"imgui buffer is too small"};
            }

            TRY_EXPECTED_VOID(frameData.imguiBuffer->copyToBuffer(idxData, currImguiBufferOffset));

            vkCmdBindIndexBuffer(frameData.commandBuffer,  //
                                 frameData.imguiBuffer->buffer(),  //
                                 currImguiBufferOffset,  //
                                 VK_INDEX_TYPE_UINT16);

            currImguiBufferOffset += indexDataSize;
            currImguiBufferOffset +=
                padding(currImguiBufferOffset, deviceProperties.properties.limits.minStorageBufferOffsetAlignment);
        }

        auto const bufferBindingPushConstData{utils::get_push_constant_data(vertexDataBinding)};

        vkCmdPushConstants(frameData.commandBuffer,  //
                           pipelineLayout,  //
                           VK_SHADER_STAGE_ALL,  //
                           static_cast<uint32_t>(imguiFramePushConstData.size()),  //
                           static_cast<uint32_t>(bufferBindingPushConstData.size()),  //
                           bufferBindingPushConstData.data());

        for (auto const& drawCmd : cmdList->CmdBuffer) {
            {
                if (drawCmd.GetTexID() == VK_NULL_HANDLE) {
                    return std::unexpected{"failed to find imgui texture"};
                }

                VkImageView imguiImageView{drawCmd.GetTexID()};

                VkDescriptorImageInfo imageInfo{};
                imageInfo.sampler = sampler;
                imageInfo.imageView = imguiImageView;
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

                uint32_t imguiImageBinding = writeDescriptorSet.dstArrayElement;

                auto const imguiImageBindingPushConstData{utils::get_push_constant_data(imguiImageBinding)};

                vkCmdPushConstants(
                    frameData.commandBuffer,  //
                    pipelineLayout,  //
                    VK_SHADER_STAGE_ALL,  //
                    static_cast<uint32_t>(imguiFramePushConstData.size() + bufferBindingPushConstData.size()),  //
                    static_cast<uint32_t>(imguiImageBindingPushConstData.size()),  //
                    imguiImageBindingPushConstData.data());
            }

            ImVec2 const clipOffset{drawData->DisplayPos};
            ImVec2 const clipScale{drawData->FramebufferScale};
            auto const fbWidth{static_cast<uint32_t>(drawData->DisplaySize.x * clipScale.x)};
            auto const fbHeight{static_cast<uint32_t>(drawData->DisplaySize.y * clipScale.y)};

            ImVec2 const clipMin{std::max(0.0f, (drawCmd.ClipRect.x - clipOffset.x) * clipScale.x),
                                 std::max(0.0f, (drawCmd.ClipRect.y - clipOffset.y) * clipScale.y)};

            ImVec2 const clipMax{
                std::min(static_cast<float>(fbWidth), (drawCmd.ClipRect.z - clipOffset.x) * clipScale.x),
                std::min(static_cast<float>(fbHeight), (drawCmd.ClipRect.w - clipOffset.y) * clipScale.y)};

            if ((clipMax.x <= clipMin.x) || (clipMax.y <= clipMin.y)) {
                continue;
            }

            VkRect2D scissor{};
            scissor.offset.x = static_cast<int32_t>(clipMin.x);
            scissor.offset.y = static_cast<int32_t>(clipMin.y);
            scissor.extent.width = static_cast<uint32_t>(clipMax.x - clipMin.x);
            scissor.extent.height = static_cast<uint32_t>(clipMax.y - clipMin.y);

            vkCmdSetScissor(frameData.commandBuffer, 0, 1, &scissor);

            vkCmdDrawIndexed(frameData.commandBuffer,  //
                             drawCmd.ElemCount,  //
                             1,
                             drawCmd.IdxOffset,  //
                             static_cast<int32_t>(drawCmd.VtxOffset),  //
                             0);
        }
    }

    return {};
}

}  // namespace graphics::vk::impl
