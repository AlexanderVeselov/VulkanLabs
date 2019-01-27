#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform UBO
{
    vec3 data;
} ubo;

void main() {
    outColor = vec4(fragColor + ubo.data, 1.0);
}
