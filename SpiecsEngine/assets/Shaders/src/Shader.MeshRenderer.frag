#version 460

// frag input
layout(location = 0) in vec3 fragColor;

// push constant
layout(push_constant) uniform Push{
	mat4 model;
} push;

// uniform buffer
layout(set = 1, binding = 0) uniform UniformBuffer0 {
    mat4 projection;
    mat4 view;
} ubo0;

layout(set = 1, binding = 1) uniform UniformBuffer {
    mat4 projection;
    mat4 view;
} ubo;

// frag output
layout(location = 0) out vec4 outColor;

// constant

// main
void main()
{
    float x = ubo.projection[0].x + ubo0.view[0].x;

    outColor = vec4(fragColor, 1.0);
}