#version 460

// vertex input
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;
layout(location = 3) in vec2 texCoord;

// vertex output
layout(location = 0) out struct FragInput {
    vec3 localPos;
} vertOut;

// push constant
layout(push_constant) uniform Push {
    mat4 model;
    int entityID;
} push;

// uniform buffer
layout(set = 0, binding = 0) uniform View {
    mat4 projection;
    mat4 view;
    mat4 inView;
    vec4 sceneTextureSize;
    vec4 windowSize;
} view;

layout(set = 0, binding = 1) uniform SpiecsInput {
    vec4 mousePos;
    float gameTime;
    float frameTime;
} spiecsInput;

// constant

// main
void main()
{
    vertOut.localPos = position;
    gl_Position = view.projection * view.view * push.model * vec4(position, 1.0);
}
