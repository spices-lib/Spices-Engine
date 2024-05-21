#version 460

// frag input
layout(location = 0) in struct FragInput {
    vec2 texCoord;
} fragInput;

// frag output
layout(location = 0) out vec4 outSceneColor;
layout(location = 1) out float outID;

// push constant
layout(push_constant) uniform Push {
    mat4 model;
    int entityID;
} push;

// uniform buffer
layout(set = 1, binding = 0) uniform sampler2D samplers;

// constant

// main
void main()
{
    vec4 texColor = texture(samplers, fragInput.texCoord);

    if (texColor.w < 0.01f) discard;

    outSceneColor = texColor;
    outID = push.entityID;
}