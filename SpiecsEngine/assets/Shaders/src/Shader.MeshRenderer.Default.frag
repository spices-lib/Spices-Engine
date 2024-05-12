#version 460

// frag input
layout(location = 0) in struct FragInput {
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 texCoord;
} fragInput;

// frag output
layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 outNormal;
layout(location = 2) out vec4 outSpecular;
layout(location = 3) out float outID;

// push constant
layout(push_constant) uniform Push{
	mat4 model;
	int entityID;
} push;

// uniform buffer
struct PointLight {
    vec3 color;
    vec3 position;
    float intensity;
    float constantf;
    float linear;
    float quadratic;
};

layout(set = 1, binding = 0) uniform sampler2D samplers[3];

layout(set = 2, binding = 0) uniform DirectionalLight{
    vec3 direction;
    vec3 color;
    float intensity;
} directionalLight;

layout(set = 2, binding = 1) uniform PointLights {
    PointLight pointLight[10];
} pointLights;

// constant
#define ALBEDO 0
#define NORMAL 1
#define SPECULAR 2

// main
void main()
{
    outColor = texture(samplers[ALBEDO], fragInput.texCoord);
    outNormal = vec4(fragInput.normal, 1.0f);
    outSpecular = texture(samplers[SPECULAR], fragInput.texCoord);
    outID = push.entityID;
}