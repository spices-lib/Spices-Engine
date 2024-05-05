#version 460

// frag input
layout(location = 0) in struct FragInput {
    vec3 localPos;
} fragInput;

// frag output
layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 outNormal;
layout(location = 2) out float outID;

// push constant
layout(push_constant) uniform Push {
    mat4 model;
    int entityID;
} push;

// uniform buffer
layout(set = 1, binding = 0) uniform sampler2D samplers[1];

// constant
const int diffuseTexture = 0;
const vec2 invAtan = vec2(0.1591, 0.3183);

// functions
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(-v.z, v.x), asin(-v.y));
    uv *= invAtan;
    uv += 0.5;

    return uv;
}

// main
void main()
{
    vec2 uv = SampleSphericalMap(normalize(fragInput.localPos)); // make sure to normalize localPos
    outColor = texture(samplers[diffuseTexture], uv);
    outNormal = vec4(0.0f);
    outID = push.entityID;
}
