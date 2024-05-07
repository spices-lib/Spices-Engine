#version 460

// vertex input
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;
layout(location = 3) in vec2 texCoord;

// push constant
layout(push_constant) uniform Push {
    mat4 model;
    int entityID;
} push;

// uniform buffer
layout(set = 0, binding = 0) uniform UniformBuffer {
    mat4 projection;
    mat4 view;
} view;

// constant


// main
void main()
{
    gl_Position = view.projection * view.view * push.model * vec4(position, 1.0);
}