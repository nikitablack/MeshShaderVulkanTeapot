#include <graphics/vk/impl/begin_compose_rendering.hpp>
#include <graphics/vk/utils/barrier_helper.hpp>
#include <vulkan/utility/vk_struct_helper.hpp>

namespace graphics::vk::impl {

auto begin_compose_rendering(VkCommandBuffer commandBuffer,  //
                             VkImage swapchainImage,  //
                             VkImageView swapchainImageView,  //
                             VkExtent2D const& surfaceExtent  //
                             ) noexcept -> void {
    utils::set_image_barrier(commandBuffer,  //
                             swapchainImage,  //
                             VK_IMAGE_ASPECT_COLOR_BIT,  //
                             VK_PIPELINE_STAGE_2_NONE,  //
                             VK_ACCESS_2_NONE,  //
                             VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,  //
                             VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,  //
                             VK_IMAGE_LAYOUT_UNDEFINED,  //
                             VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

    VkClearColorValue clearColor{};
    clearColor.float32[0] = 0.0f;  // r
    clearColor.float32[1] = 0.5f;  // g
    clearColor.float32[2] = 0.0f;  // b
    clearColor.float32[3] = 1.0f;  // a

    VkClearValue clearValue{};
    clearValue.color = clearColor;

    VkRenderingAttachmentInfo attachmentInfo = vku::InitStructHelper{};
    attachmentInfo.imageView = swapchainImageView;
    attachmentInfo.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    attachmentInfo.resolveMode = VK_RESOLVE_MODE_NONE;
    attachmentInfo.resolveImageView = VK_NULL_HANDLE;
    attachmentInfo.resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachmentInfo.clearValue = clearValue;

    VkRect2D renderArea{};
    renderArea.offset = VkOffset2D{0, 0};
    renderArea.extent = surfaceExtent;

    VkRenderingInfo renderingInfo = vku::InitStructHelper{};
    renderingInfo.flags = 0;
    renderingInfo.renderArea = renderArea;
    renderingInfo.layerCount = 1;
    renderingInfo.viewMask = 0;
    renderingInfo.colorAttachmentCount = 1;
    renderingInfo.pColorAttachments = &attachmentInfo;
    renderingInfo.pDepthAttachment = nullptr;
    renderingInfo.pStencilAttachment = nullptr;

    vkCmdBeginRendering(commandBuffer, &renderingInfo);
}

}  // namespace graphics::vk::impl