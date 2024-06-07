/************************************Pre Compile*******************************************/

#version 460

/*****************************************************************************************/

/************************************FragMent Input***************************************/

/**
* @brief Fragment Shader Input From Vertex Shader.
*/
layout(location = 0) in struct FragInput {
	vec2 texCoord;         /*Frag UV*/
} fragInput;

/*****************************************************************************************/

/************************************FragMent Output**************************************/

/**
* @brief Fragment Shader Output to FrameBuffer.
*/
layout(location = 0) out vec4 outColor;  /*SceneColor Attachment*/

/*****************************************************************************************/

/*************************************Pre Renderer Data***********************************/

/**
* @brief Global View Struct.
*/
layout(set = 0, binding = 0) uniform View {
	mat4 projection;          /*Projection Matrix from major Camera Entity*/
	mat4 view;                /*View Matrix from major Camera Entity*/
	mat4 inView;              /*Inverse View Matrix from major Camera Entity*/
	vec4 sceneTextureSize;    /*Scene Texture Size*/
	vec4 windowSize;          /*Application Window Size*/
} view;

/**
* @brief Application Inout Struct.
*/
layout(set = 0, binding = 1) uniform SpiecsInput {
	vec4 mousePos;            /*Mouse Postion and inverse position*/
	float gameTime;           /*Application Run time since start*/
	float frameTime;          /*Duration time since last frame*/
} spiecsInput;

/*****************************************************************************************/

/********************************Specific Renderer Data***********************************/

/**
* @brief GBuffer struct like that:
* 0 - Diffuse.
* 1 - Normal.
* 2 - Specular.
* 3 - Position.
* 4 - Depth.
*/

const int DIFFUSE  = 0;
const int NORMAL   = 1;
const int SPECULAR = 2;
const int POSITION = 3;
const int DEPTH    = 4;

layout(input_attachment_index = 0, set = 1, binding = 0) uniform subpassInput GBuffer[5];

/**
* @brief PointLight Struct
*/
struct DirectionalLight {
	vec3  direction;          /*Directional Light Direction*/
	vec3  color;              /*Directional Light Color*/
	float intensity;          /*Directional Light Intensity*/
};

/**
* @brief PointLight Struct
*/
struct PointLight {
	vec3  position;           /*Point Light Position*/
	vec3  color;              /*Point Light Color*/
	float intensity;          /*Point Light Intensity*/
	float constantf;          /**/
	float linear;             /**/
	float quadratic;          /**/
};

/**
* @brief Light Buffer Struct.
*/
layout(std140, set = 1, binding = 1) buffer LightsBuffer {
	DirectionalLight directionalLight;
	PointLight pointLights[];    /*Point Light Array*/
};

/*****************************************************************************************/

/*************************************Functions*******************************************/

/**
* @brief Calculate Point Light Lighting.
*/
vec4 CalculatePointLight(PointLight light, vec3 normal, vec3 position, vec3 cameraDirection);

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
	/**
	* @brief World Normal.
	*/
	vec3 normal = subpassLoad(GBuffer[NORMAL]).rgb;
	normal = normalize((normal - vec3(0.5f)) * 2.0f);

	///**
	//* @brief World Position.
	//*/
	//vec3 postion = subpassLoad(GBuffer[POSITION]).rgb;

	///**
	//* @brief Major Camera World Position.
	//*/
	//vec3 cameraPosition = view.inView[3].xyz;

	///**
	//* @brief Fragment Postion Direction to Major Camera.
	//*/
	//vec3 cameraDirection = normalize(cameraPosition - postion);

	///**
	//* @brief Init Color with diffuse.
	//*/
	//vec4 color = subpassLoad(GBuffer[DIFFUSE]);
	////vec4 color = vec4(0.0f);
	//for (int i = 0; i < 2; i++)
	//{
	//	color += CalculatePointLight(pointLights[i], normal, postion, cameraDirection);
	//}

	//outColor = color;

	float diff = max(dot(normal, normalize(directionalLight.direction)), 0.0f);
	vec3 diffuse = diff * directionalLight.color * directionalLight.intensity;


	vec3 result = (0.5f + diffuse) * subpassLoad(GBuffer[DIFFUSE]).xyz;
	outColor = vec4(result, 1.0f);
}

/*****************************************************************************************/

vec4 CalculatePointLight(PointLight light, vec3 normal, vec3 position, vec3 cameraDirection) {

	/**
	* @brief Fragment Postion Direction to Light.
	*/
	vec3 lightDirection = normalize(light.position - position);

	/**
	* @brief Cosine theta between world normal and light direction.
	*/
	float diff = max(dot(normal, lightDirection), 0.0f);

	/***/
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float spec = pow(max(dot(cameraDirection, reflectDirection), 0.0f), 1.0f);

	float distance = length(light.position - position);
	float attenuation = 1.0f / (light.constantf + light.linear * distance + light.quadratic * (distance * distance));

	vec4 ambient = vec4(0.3f);
	vec4 diffuse = vec4(light.color, 1.0f) * diff * light.intensity;
	vec4 specular = vec4(light.color, 1.0f) * spec;

	vec4 diffSamp = subpassLoad(GBuffer[DIFFUSE]);
	diffuse  *= diffSamp;
	ambient  *= diffSamp;
	specular *= vec4(subpassLoad(GBuffer[SPECULAR]).rgb, diffuse.a);

	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}