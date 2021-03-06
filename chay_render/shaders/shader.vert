#version 450
#extension GL_ARB_separate_shader_objects : enable

out gl_PerVertex {
    vec4 gl_Position;
};

layout(binding = 0) uniform UBO
{
    vec3 data;
} ubo;

layout(location = 0) out vec3 fragColor;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

void main() {
    gl_Position = vec4(inPosition, 1.0f);
    fragColor = ubo.data;
}
