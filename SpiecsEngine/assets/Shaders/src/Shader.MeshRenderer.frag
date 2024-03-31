#version 460

// frag input
layout(location = 0) in vec3 fragColor;

// push constant

// uniform buffer

// frag output
layout(location = 0) out vec4 outColor;

// main
void main()
{
    outColor = vec4(fragColor, 1.0);
}