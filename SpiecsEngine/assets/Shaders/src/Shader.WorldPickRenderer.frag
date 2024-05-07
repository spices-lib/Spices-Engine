#version 460

// frag input

// frag output
layout(location = 0) out float outPick;

// push constant
layout(push_constant) uniform Push {
    mat4 model;
    int entityID;
} push;

// uniform buffer

// constant

// main
void main()
{
    outPick = push.entityID;
}