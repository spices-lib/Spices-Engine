#version 460

// frag input
layout(location = 0) in vec3 fragColor;

// push constant
layout(push_constant) uniform Push{
	mat4 ModelMatrix;
} push;

// uniform buffer
layout(set = 0, binding = 0) uniform UniformBuffer {
    mat4 projection;
    mat4 view;
} ubo;

// frag output
layout(location = 0) out vec4 outColor;

// main
void main()
{
    outColor = vec4(fragColor, 1.0);
}