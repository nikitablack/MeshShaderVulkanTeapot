#version 450
#extension GL_EXT_nonuniform_qualifier : require

layout(push_constant) uniform PushConstants
{
    uint renderTargetDescriptorIndex;
} pushConstants;

layout(set = 1, binding = 0) uniform sampler2D textureDescriptors[];

layout(location = 0) in vec2 inUv;

layout(location = 0) out vec4 outColor;

void main()
{
    outColor = texture(nonuniformEXT(textureDescriptors[pushConstants.renderTargetDescriptorIndex]), inUv);
}
