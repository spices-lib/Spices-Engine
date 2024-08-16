/**
* @file Shader.SceneComposeRenderer.SceneCompose.Default.frag.
* @brief This Shader Defines SceneCompose Renderer SceneCompose SubPass Default Fragemt Shader Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive : enable     /* @brief Enable include Macro. */

#include "Header/ShaderCommon.h"
#include "Header/ShaderPreRendererLayout.glsl"
#include "Header/ShaderFunctionLibrary.glsl"

/*****************************************************************************************/

/************************************Fragment Input***************************************/

/**
* @brief Fragment Shader Input From Vertex Shader.
*/
layout(location = 0) in struct FragInput 
{
	vec2 texCoord;                         /*Fragmet UV*/
} 
fragInput;

/*****************************************************************************************/

/************************************Fragment Output**************************************/

/**
* @brief Fragment Shader Output to FrameBuffer.
*/
layout(location = 0) out vec4 outSceneColor;    /*SceneColor Attachment*/

/*****************************************************************************************/

/********************************Specific Renderer Data***********************************/

/**
* @brief GBuffer struct like that:
* 0 - Albedo.
* 1 - Normal.
* 2 - Roughness.
* 3 - Metallic.
* 4 - Position.
* 5 - Depth.
*/

#define ALBEDO    0
#define NORMAL    1
#define ROUGHNESS 2
#define METALLIC  3
#define POSITION  4
#define DEPTH     5

/**
* @brief Subpass Input Attachments.
*/
layout(input_attachment_index = 0, set = 2, binding = 0) uniform subpassInput GBuffer[6];

/**
* @brief DirectionalLight Buffer in World.
*/
layout(set = 3, binding = 0, scalar) readonly buffer DLightBuffer 
{
	DirectionalLight i[];   /* @see DirectionalLight. */
} 
dLightBuffer;

/**
* @brief PointLight Buffer in World.
*/
layout(set = 3, binding = 1, scalar) readonly buffer PLightBuffer 
{
	PointLight i[];         /* @see PointLight. */
} 
pLightBuffer;

/*****************************************************************************************/

/******************************************Functions**************************************/

/**
* @brief Get MaterialAttributes from GBuffer.
* @return Returns the Pixel MaterialAttributes.
* @see MaterialAttributes.
*/
GBufferPixel GetGBufferPixel();

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
	GBufferPixel gbp = GetGBufferPixel();
	
	vec4 origin = view.inView * vec4(0.0f, 0.0f, 0.0f, 1.0f);
    vec3 V = normalize(origin.xyz - gbp.position);
    
    vec3 col = BRDF_Diffuse_Lambert(gbp.albedo) * PI;
    
    for(int i = 0; i < pLightBuffer.i.length(); i++)
    {
    	PointLight light = pLightBuffer.i[i];
    	if(light.intensity < -500.0f) break;
    	 
    	vec3 lpos = light.position;
        vec3 L = normalize(lpos - gbp.position);
        float tMax = length(lpos - gbp.position);
        
        float attenuation = 1.0f / (light.constantf + light.linear * tMax + light.quadratic * tMax * tMax);
        col += BRDF_Specular_CookTorrance(L, V, gbp.normal, light.color * light.intensity * attenuation, gbp.albedo, gbp.metallic, gbp.roughness);
    }
    
	for(int i = 0; i < dLightBuffer.i.length(); i++)
	{
		DirectionalLight light = dLightBuffer.i[i];
        if(light.intensity < -500.0f) break;
        
        vec4 dir4 = light.rotationMatrix * vec4(1.0f, 0.0f, 0.0f, 1.0f);
        vec3 L = dir4.xyz;
        
        col += BRDF_Specular_CookTorrance(L, V, gbp.normal, light.color * light.intensity, gbp.albedo, gbp.metallic, gbp.roughness);
	}

	outSceneColor = vec4(col, 1.0f);
}

/*****************************************************************************************/

GBufferPixel GetGBufferPixel()
{
	GBufferPixel gbp;
	
	gbp.albedo      = subpassLoad(GBuffer[ALBEDO]).xyz;
	gbp.normal      = (subpassLoad(GBuffer[NORMAL]).xyz - vec3(0.5f)) * 2.0f;
	gbp.roughness   = subpassLoad(GBuffer[ROUGHNESS]).x;
	gbp.metallic    = subpassLoad(GBuffer[METALLIC]).x;
	gbp.position    = subpassLoad(GBuffer[POSITION]).xyz;
	gbp.depth       = subpassLoad(GBuffer[DEPTH]).x;
	
	return gbp;
}