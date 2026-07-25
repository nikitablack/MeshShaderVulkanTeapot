#version 450
#extension GL_EXT_nonuniform_qualifier : require

layout(push_constant) uniform PushConst
{
    vec2 scale;
    vec2 translate;
    uint vertexDataDescriptorIndex;
    uint imguiImageDescriptorIndex;
} pushConstants;

struct Vertex
{
    float x;
    float y;
    float u;
    float v;
    uint color;
};

layout(set = 0, binding = 0) readonly buffer VertexData
{
    Vertex data[];
} vertexData[];

layout(location = 0) out vec2 outUv;
layout(location = 1) out vec4 outColor;

out gl_PerVertex
{
    vec4 gl_Position;
};

vec3 linear_from_srgb(vec3 srgb){
    bvec3 cutoff = lessThan(srgb, vec3(0.04045));
    vec3 lower = srgb / vec3(12.92);
    vec3 higher = pow((srgb + vec3(0.055)) / vec3(1.055), vec3(2.4));
    return mix(higher, lower, cutoff);
}

vec4 linear_from_srgba(vec4 srgba){
    return vec4(linear_from_srgb(srgba.rgb), srgba.a);
}

void main()
{
    Vertex vertex = vertexData[nonuniformEXT(pushConstants.vertexDataDescriptorIndex)].data[gl_VertexIndex];

    vec2 pos = vec2(vertex.x, vertex.y);
    gl_Position = vec4(pos * pushConstants.scale + pushConstants.translate, 0.0f, 1.0f);

    outUv = vec2(vertex.u, vertex.v);

    vec4 colorSrgba = vec4(vertex.color & 0xFF, (vertex.color >> 8) & 0xFF, (vertex.color >> 16) & 0xFF, (vertex.color >> 24) & 0xFF);
    colorSrgba /= 0xFF;
    outColor = linear_from_srgba(colorSrgba);
}