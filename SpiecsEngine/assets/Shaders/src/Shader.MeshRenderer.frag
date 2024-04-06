#version 460

// frag input
layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

// frag output
layout(location = 0) out vec4 outColor;

// push constant
layout(push_constant) uniform Push{
	mat4 model;
} push;

// uniform buffer
//layout(set = 1, binding = 0) uniform UniformBuffer {
//    mat4 projection;
//    mat4 view;
//} ubo;

layout(set = 1, binding = 0) uniform sampler2D texSampler;

// constant

// main
void main()
{
    outColor = texture(texSampler, fragTexCoord);
}