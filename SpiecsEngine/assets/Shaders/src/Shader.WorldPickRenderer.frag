#version 460

// frag input
layout(location = 0) in struct FragInput {
    vec2 texCoord;
} fragInput;

// frag output
layout(location = 0) out float outPick;

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
    outPick = push.entityID;

    /*float alpha = texture(samplers, fragInput.texCoord).w;
    if (alpha < 0.01f) discard;
    outPick = push.entityID; */
}