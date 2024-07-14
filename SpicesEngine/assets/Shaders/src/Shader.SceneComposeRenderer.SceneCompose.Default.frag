/**
* @file Shader.SceneComposeRenderer.SceneCompose.Default.frag.
* @brief This Shader Defines SceneCompose Renderer SceneCompose SubPass Default Fragemt Shader Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive : enable     /* @brief Enable include Macro. */

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

/**
* @brief Subpass Input Attachments.
*/
layout(input_attachment_index = 0, set = 1, binding = 0) uniform subpassInput GBuffer[5];

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
	outColor = subpassLoad(GBuffer[DIFFUSE]);
}

/*****************************************************************************************/