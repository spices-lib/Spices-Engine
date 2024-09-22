/**
* @file Shader.SceneComposeRenderer.SceneCompose.Default.frag.
* @brief This Shader Defines SceneCompose Renderer SceneCompose SubPass Default Fragemt Shader Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive : enable    /* @brief Enable include Macro. */

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
layout(location = 0) out vec4  outColor;           /* @brief SceneColor Attachment. */
layout(location = 1) out float outEntityID;        /* @brief ID Attachment.         */
layout(location = 2) out vec4  outTriangleID;      /* @brief ID Attachment.         */

/*****************************************************************************************/

/********************************Specific Renderer Data***********************************/

/**
* @brief Storage Image written by RayTracing Renderer.
*/
layout(set = 2, binding = 0, rgba32f) uniform image2D RayImage;
layout(set = 2, binding = 1, r32f) uniform image2D RayID[];

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
	ivec2 uv     = ivec2(view.sceneTextureSize.xy * fragInput.texCoord.xy);
	outColor     = imageLoad(RayImage, uv);
	outEntityID  = imageLoad(RayID[0], uv).x;

	uint  triangleSeed  = uint(imageLoad(RayID[1], uv).x);
	float trianglerand0 = rnd(triangleSeed);
	float trianglerand1 = rnd(triangleSeed);
	float trianglerand2 = rnd(triangleSeed);

	outTriangleID = vec4(trianglerand0, trianglerand1, trianglerand2, 1.0f);
}

/*****************************************************************************************/