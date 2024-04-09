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

// push constant
layout(push_constant) uniform Push{
	mat4 model;
	int entityID;
	int meshpackID;
} push;

// uniform buffer
//layout(set = 1, binding = 0) uniform UniformBuffer {
//    mat4 projection;
//    mat4 view;
//} ubo;

struct PointLight {
    vec3 color;
    vec3 position;
    float intensity;
    float constantf;
    float linear;
    float quadratic;
};

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
};

layout(set = 1, binding = 0) uniform sampler2D samplers[3];
layout(set = 2, binding = 0) uniform Light {
    DirectionalLight directionalLight;
    PointLight pointLights[10];
} light;


// constant


const int diffuseTexture = 0;
const int normalTexture = 1;
const int specularTexture = 2;

// main
void main()
{
    outColor = texture(samplers[diffuseTexture], fragInput.texCoord);
	//outColor.xyz = pow(outColor.xyz, vec3(1.0 / 2.2));
}