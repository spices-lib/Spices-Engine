#version 460

// vertex input
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;
layout(location = 3) in vec2 texCoord;

// vertex output
layout(location = 0) out struct FragInput {
    vec3 texCoord;
} vertOut;

// push constant
layout(push_constant) uniform Push {
    mat4 model;
    int entityID;
    int meshpackID;
} push;

// uniform buffer
layout(set = 0, binding = 0) uniform UniformBuffer {
    mat4 projection;
    mat4 view;
} ubo;

// constant


// main
void main()
{
    vertOut.texCoord = position;
    gl_Position = ubo.projection * ubo.view * push.model * vec4(position, 1.0);
}
