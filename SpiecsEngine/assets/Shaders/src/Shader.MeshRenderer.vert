#version 460

// vertex input
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;
layout(location = 3) in vec2 texCoord;

// push constant

// uniform buffer

// vertex output
layout(location = 0) out vec3 fragColor;

// main
void main()
{
    gl_Position = vec4(position, 1.0);
    fragColor = color;
}
