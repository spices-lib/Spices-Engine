/**
* @file Shader.SceneComposeRenderer.SceneCompose.Default.frag.
* @brief This Shader Defines SceneCompose Renderer SceneCompose SubPass Default Fragemt Shader Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive : enable    /* @brief Enable include Macro. */

#include "Header/ShaderPreRendererLayout.glsl"

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
layout(location = 0) out vec4 outColor;    /* @brief SceneColor Attachment. */
layout(location = 1) out float outID;      /* @brief ID Attachment.         */

/*****************************************************************************************/

/********************************Specific Renderer Data***********************************/

layout(set = 2, binding = 0, rgba32f) uniform image2D RayImage;
layout(set = 2, binding = 1, r32f) uniform image2D RayID;

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
	ivec2 uv  = ivec2(view.sceneTextureSize.xy * fragInput.texCoord.xy);
	outColor  = imageLoad(RayImage, uv);
	outID     = imageLoad(RayID, uv).x;
}

/*****************************************************************************************/