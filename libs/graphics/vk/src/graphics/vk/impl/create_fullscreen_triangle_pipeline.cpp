#include <fmt/core.h>

#include <array>
#include <cmrc/cmrc.hpp>
#include <graphics/vk/impl/create_fullscreen_triangle_pipeline.hpp>
#include <graphics/vk/utils/create_shader_module.hpp>
#include <utils/try_expected.hpp>
#include <vulkan/utility/vk_struct_helper.hpp>

CMRC_DECLARE(shaders);

namespace graphics::vk::impl {

[[nodiscard]] auto create_fullscreen_triangle_pipeline(VkDevice device,  //
                                                       VkPipelineLayout pipelineLayout,  //
                                                       VkFormat surfaceFormat  //
                                                       ) noexcept -> std::expected<VkPipeline, std::string> {
    fmt::println("creating fullscreen triangle pipeline");

    auto const fs{cmrc::shaders::get_filesystem()};
    auto const vertexShaderSpirv{fs.open("fullscreen_triangle.vert.spv")};
    auto const fragmentShaderSpirv{fs.open("fullscreen_triangle.frag.spv")};

    TRY_EXPECTED(auto const vertexShaderModule,
                 utils::create_shader_module(
                     device,  //
                     std::span<std::byte const>{reinterpret_cast<std::byte const*>(vertexShaderSpirv.cbegin()),
                                                vertexShaderSpirv.size()}));

    TRY_EXPECTED(auto const fragmentShaderModule,
                 utils::create_shader_module(
                     device,  //
                     std::span<std::byte const>{reinterpret_cast<std::byte const*>(fragmentShaderSpirv.cbegin()),
                                                fragmentShaderSpirv.size()}));

    std::array<VkPipelineShaderStageCreateInfo, 2> shaderStageInfos{};
    shaderStageInfos[0] = vku::InitStructHelper{};
    shaderStageInfos[0].flags = 0;
    shaderStageInfos[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStageInfos[0].module = vertexShaderModule;
    shaderStageInfos[0].pName = "main";
    shaderStageInfos[0].pSpecializationInfo = nullptr;

    shaderStageInfos[1] = vku::InitStructHelper{};
    shaderStageInfos[1].flags = 0;
    shaderStageInfos[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStageInfos[1].module = fragmentShaderModule;
    shaderStageInfos[1].pName = "main";
    shaderStageInfos[1].pSpecializationInfo = nullptr;

    VkPipelineInputAssemblyStateCreateInfo inputAssemblerInfo = vku::InitStructHelper{};
    inputAssemblerInfo.flags = 0;
    inputAssemblerInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblerInfo.primitiveRestartEnable = VK_FALSE;

    VkPipelineRasterizationStateCreateInfo rasterizationInfo = vku::InitStructHelper{};
    rasterizationInfo.flags = 0;
    rasterizationInfo.depthClampEnable = VK_FALSE;
    rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
    rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizationInfo.depthBiasEnable = VK_FALSE;
    rasterizationInfo.depthBiasConstantFactor = 0.0f;
    rasterizationInfo.depthBiasClamp = 0.0f;
    rasterizationInfo.depthBiasSlopeFactor = 0.0f;
    rasterizationInfo.lineWidth = 1.0f;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    VkPipelineColorBlendStateCreateInfo colorBlendInfo = vku::InitStructHelper{};
    colorBlendInfo.flags = 0;
    colorBlendInfo.logicOpEnable = VK_FALSE;
    colorBlendInfo.logicOp = VK_LOGIC_OP_CLEAR;
    colorBlendInfo.attachmentCount = 1;
    colorBlendInfo.pAttachments = &colorBlendAttachment;
    colorBlendInfo.blendConstants[0] = 0.0f;
    colorBlendInfo.blendConstants[1] = 0.0f;
    colorBlendInfo.blendConstants[2] = 0.0f;
    colorBlendInfo.blendConstants[3] = 0.0f;

    std::array<VkDynamicState, 2> dynamicStates{VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

    VkPipelineDynamicStateCreateInfo dynamicInfo = vku::InitStructHelper{};
    dynamicInfo.flags = 0;
    dynamicInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicInfo.pDynamicStates = dynamicStates.data();

    VkViewport viewport{};
    VkRect2D scissor{};

    VkPipelineViewportStateCreateInfo viewportInfo = vku::InitStructHelper{};
    viewportInfo.flags = 0;
    viewportInfo.viewportCount = 1;
    viewportInfo.pViewports = &viewport;
    viewportInfo.scissorCount = 1;
    viewportInfo.pScissors = &scissor;

    VkPipelineMultisampleStateCreateInfo multisampleInfo = vku::InitStructHelper{};
    multisampleInfo.flags = 0;
    multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampleInfo.sampleShadingEnable = VK_FALSE;
    multisampleInfo.minSampleShading = 1.0f;
    multisampleInfo.pSampleMask = nullptr;
    multisampleInfo.alphaToCoverageEnable = VK_FALSE;
    multisampleInfo.alphaToOneEnable = VK_FALSE;

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = vku::InitStructHelper{};
    vertexInputInfo.flags = 0;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr;

    VkStencilOpState stencilOpState{};
    stencilOpState.failOp = VK_STENCIL_OP_KEEP;
    stencilOpState.passOp = VK_STENCIL_OP_KEEP;
    stencilOpState.depthFailOp = VK_STENCIL_OP_KEEP;
    stencilOpState.compareOp = VK_COMPARE_OP_NEVER;
    stencilOpState.compareMask = 0;
    stencilOpState.writeMask = 0;
    stencilOpState.reference = 0;

    VkPipelineDepthStencilStateCreateInfo depthStencilInfo = vku::InitStructHelper{};
    depthStencilInfo.flags = 0;
    depthStencilInfo.depthTestEnable = VK_FALSE;
    depthStencilInfo.depthWriteEnable = VK_FALSE;
    depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
    depthStencilInfo.stencilTestEnable = VK_FALSE;
    depthStencilInfo.front = stencilOpState;
    depthStencilInfo.back = stencilOpState;
    depthStencilInfo.minDepthBounds = 0.0f;
    depthStencilInfo.maxDepthBounds = 0.0f;

    VkPipelineRenderingCreateInfo renderingInfo = vku::InitStructHelper{};
    renderingInfo.viewMask = 0;
    renderingInfo.colorAttachmentCount = 1;
    renderingInfo.pColorAttachmentFormats = &surfaceFormat;
    renderingInfo.depthAttachmentFormat = VK_FORMAT_UNDEFINED;
    renderingInfo.stencilAttachmentFormat = VK_FORMAT_UNDEFINED;

    VkGraphicsPipelineCreateInfo pipelineInfo = vku::InitStructHelper{&renderingInfo};
    pipelineInfo.flags = 0;
    pipelineInfo.stageCount = static_cast<uint32_t>(shaderStageInfos.size());
    pipelineInfo.pStages = shaderStageInfos.data();
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssemblerInfo;
    pipelineInfo.pTessellationState = nullptr;
    pipelineInfo.pViewportState = &viewportInfo;
    pipelineInfo.pRasterizationState = &rasterizationInfo;
    pipelineInfo.pMultisampleState = &multisampleInfo;
    pipelineInfo.pDepthStencilState = &depthStencilInfo;
    pipelineInfo.pColorBlendState = &colorBlendInfo;
    pipelineInfo.pDynamicState = &dynamicInfo;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = VK_NULL_HANDLE;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    VkPipeline pipeline{VK_NULL_HANDLE};
    if (vkCreateGraphicsPipelines(device, nullptr, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS) {
        return std::unexpected{"failed to create fullscreen triangle pipeline"};
    }

    // TODO: clear with RAII
    vkDestroyShaderModule(device, vertexShaderModule, nullptr);
    vkDestroyShaderModule(device, fragmentShaderModule, nullptr);

    return pipeline;
}

}  // namespace graphics::vk::impl
