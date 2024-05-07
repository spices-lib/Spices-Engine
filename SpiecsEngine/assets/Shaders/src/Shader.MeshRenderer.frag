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
layout(location = 2) out float outID;
layout(location = 3) out float outSelectBuffer;

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

struct TextureParam {
    vec3 constant;
    int isInUse;
    float intensity;
};

layout(set = 1, binding = 0) uniform sampler2D samplers[3];

layout(set = 2, binding = 0) uniform TextureParams {
    TextureParam textureParam[3];
} textureParams;

layout(set = 2, binding = 1) uniform DirectionalLight{
    vec3 direction;
    vec3 color;
    float intensity;
} directionalLight;

layout(set = 2, binding = 2) uniform PointLights {
    PointLight pointLight[10];
} pointLights;

layout(set = 3, binding = 0) uniform SelectUBO {
    float id[20];
} selectUBO;

// constant
const int diffuseTexture = 0;
const int normalTexture = 1;
const int specularTexture = 2;

// main
void main()
{
    if (textureParams.textureParam[diffuseTexture].isInUse == 1)
    {
        outColor = texture(samplers[diffuseTexture], fragInput.texCoord) * textureParams.textureParam[diffuseTexture].intensity;
    }
    else
    {
        outColor = vec4(textureParams.textureParam[diffuseTexture].constant, 0.0f) * textureParams.textureParam[diffuseTexture].intensity;
    }
    outNormal = vec4(fragInput.normal, 1.0f);
    outID = push.entityID;

    outSelectBuffer = -1.0f;
    for (int i = 0; i < 1; i++)
    {
        if (selectUBO.id[i] < -0.5f) break;

        if (abs(push.entityID - selectUBO.id[i]) < 0.1f)
        {
            outSelectBuffer = selectUBO.id[i];
            break;
        }
    }
}