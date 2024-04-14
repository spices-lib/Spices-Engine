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
struct PointLight {
    vec3 color;
    vec3 position;
    float intensity;
    float constantf;
    float linear;
    float quadratic;
};

struct TextureParam {
    int isInUse;
    vec3 constant;
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

// constant
const int diffuseTexture = 0;
const int normalTexture = 1;
const int specularTexture = 2;

// function
//vec4 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection);
//vec4 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection);

// main
void main()
{
    outColor = texture(samplers[diffuseTexture], fragInput.texCoord);
}

//vec4 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection) {
//    float diffuseFactor = max(dot(normal, -light.direction), 0.0);
//
//    vec3 halfDirection = normalize(viewDirection - light.direction);
//    float specularFactor = pow(max(dot(halfDirection, normal), 0.0), object_ubo.shininess);
//
//    vec4 diffuse = texture(samplers[diffuseTexture], fragInput.texCoord);
//    vec4 ambient = vec4(vec3(in_dto.ambient * object_ubo.diffuse_colour), diff_samp.a);
//    vec4 diffuse = vec4(vec3(light.colour * diffuse_factor), diff_samp.a);
//    vec4 specular = vec4(vec3(light.colour * specular_factor), diff_samp.a);
//
//    diffuse *= diff_samp;
//    ambient *= diff_samp;
//    specular *= vec4(texture(samplers[SAMP_SPECULAR], in_dto.tex_coord).rgb, diffuse.a);
//}
//
//vec4 calculate_point_light(point_light light, vec3 normal, vec3 frag_position, vec3 view_direction) {
//    vec3 light_direction = normalize(light.position - frag_position);
//    float diff = max(dot(normal, light_direction), 0.0);
//
//    vec3 reflect_direction = reflect(-light_direction, normal);
//    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), object_ubo.shininess);
//
//    // Calculate attenuation, or light falloff over distance.
//    float distance = length(light.position - frag_position);
//    float attenuation = 1.0 / (light.constant_f + light.linear * distance + light.quadratic * (distance * distance));
//
//    vec4 ambient = in_dto.ambient;
//    vec4 diffuse = light.colour * diff;
//    vec4 specular = light.colour * spec;
//
//    if (in_mode == 0) {
//        vec4 diff_samp = texture(samplers[SAMP_DIFFUSE], in_dto.tex_coord);
//        diffuse *= diff_samp;
//        ambient *= diff_samp;
//        specular *= vec4(texture(samplers[SAMP_SPECULAR], in_dto.tex_coord).rgb, diffuse.a);
//    }
//
//    ambient *= attenuation;
//    diffuse *= attenuation;
//    specular *= attenuation;
//    return (ambient + diffuse + specular);
//}