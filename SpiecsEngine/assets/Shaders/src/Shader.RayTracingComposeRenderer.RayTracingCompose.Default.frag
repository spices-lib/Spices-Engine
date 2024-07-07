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

layout(set = 1, binding = 0, rgba32f) uniform image2D image;

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
	ivec2 uv = ivec2(view.sceneTextureSize.xy * fragInput.texCoord.xy);
	outColor = imageLoad(image, uv);
}

/*****************************************************************************************/