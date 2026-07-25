#version 450
#extension GL_EXT_nonuniform_qualifier : require

layout(push_constant) uniform PushConst
{
    vec2 scale;
    vec2 translate;
    uint vertexDataDescriptorIndex;
    uint imguiImageDescriptorIndex;
} pushConstants;

layout(set = 1, binding = 0) uniform sampler2D textures2d[];

layout(location = 0) in vec2 inUv;
layout(location = 1) in vec4 inColor;

layout(location = 0) out vec4 outColor;

void main()
{
    outColor = inColor * texture(textures2d[nonuniformEXT(pushConstants.imguiImageDescriptorIndex)], inUv);
}