#version 460

// frag input
layout(location = 0) in struct FragInput {
    vec3 texCoord;
} vertOut;

// frag output
layout(location = 0) out vec4 outColor;

// constant
const int diffuseTexture = 0;

// uniform buffer
layout(set = 1, binding = 0) uniform samplerCube samplers[1];

// main
void main()
{
    outColor = texture(samplers[diffuseTexture], vertOut.texCoord);
}
