/**
* @file Shader.SceneComposeRenderer.SceneCompose.Default.frag.
* @brief This Shader Defines SceneCompose Renderer SceneCompose SubPass Default Fragemt Shader Behaver.
* @author Spiecs.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive : enable

#include "Header/ShaderLayouts.glsl"

/*****************************************************************************************/

/************************************Fragment Input***************************************/

/**
* @brief Fragment Shader Input From Vertex Shader.
*/
layout(location = 0) in struct FragInput {
	vec2 texCoord;                         /*Fragmet UV*/
} fragInput;

/*****************************************************************************************/

/************************************Fragment Output**************************************/

/**
* @brief Fragment Shader Output to FrameBuffer.
*/
layout(location = 0) out vec4 outColor;    /*SceneColor Attachment*/

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

/*****************************************************************************************/

/*************************************Functions*******************************************/

/**
* @brief Calculate Point Light Lighting.
*/
//vec4 CalculatePointLight(PointLight light, vec3 normal, vec3 position, vec3 cameraDirection);

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

	//float diff = max(dot(normal, normalize(directionalLight.direction)), 0.0f);
	//vec3 diffuse = diff * directionalLight.color * directionalLight.intensity;


	//vec3 result = (0.5f + diffuse) * subpassLoad(GBuffer[DIFFUSE]).xyz;
	//outColor = vec4(result, 1.0f);
	outColor = subpassLoad(GBuffer[DIFFUSE]);
}

/*****************************************************************************************/

//vec4 CalculatePointLight(PointLight light, vec3 normal, vec3 position, vec3 cameraDirection) {
//
//	/**
//	* @brief Fragment Postion Direction to Light.
//	*/
//	vec3 lightDirection = normalize(light.position - position);
//
//	/**
//	* @brief Cosine theta between world normal and light direction.
//	*/
//	float diff = max(dot(normal, lightDirection), 0.0f);
//
//	/***/
//	vec3 reflectDirection = reflect(-lightDirection, normal);
//	float spec = pow(max(dot(cameraDirection, reflectDirection), 0.0f), 1.0f);
//
//	float distance = length(light.position - position);
//	float attenuation = 1.0f / (light.constantf + light.linear * distance + light.quadratic * (distance * distance));
//
//	vec4 ambient = vec4(0.3f);
//	vec4 diffuse = vec4(light.color, 1.0f) * diff * light.intensity;
//	vec4 specular = vec4(light.color, 1.0f) * spec;
//
//	vec4 diffSamp = subpassLoad(GBuffer[DIFFUSE]);
//	diffuse  *= diffSamp;
//	ambient  *= diffSamp;
//	specular *= vec4(subpassLoad(GBuffer[SPECULAR]).rgb, diffuse.a);
//
//	ambient  *= attenuation;
//	diffuse  *= attenuation;
//	specular *= attenuation;
//
//	return (ambient + diffuse + specular);
//}