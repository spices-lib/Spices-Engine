/**
* @file Shader.SceneComposeRenderer.SceneCompose.Default.frag.
* @brief This Shader Defines SceneCompose Renderer SceneCompose SubPass Default Fragemt Shader Behaver.
* @author Spiecs.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive : enable

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
layout(location = 0) out vec4 outColor;    /*SceneColor Attachment*/

/*****************************************************************************************/

/********************************Specific Renderer Data***********************************/

layout(set = 1, binding = 0, rgba32f) uniform image2D image;

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
	ivec2 uv = ivec2(view.sceneTextureSize.xy * fragInput.texCoord.xy);
	vec4 col = imageLoad(image, uv);
	col = pow(col, vec4(1.0f / 2.2f));
	outColor = col;
}

/*****************************************************************************************/