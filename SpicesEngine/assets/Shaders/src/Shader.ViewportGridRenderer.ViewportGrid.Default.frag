/**
* @file Shader.ViewportGridRenderer.ViewportGrid.Default.frag.
* @brief This Shader Defines ViewportGrid Renderer ViewportGrid SubPass Default Fragemt Shader Behaver.
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
	vec2 texCoord;                        /* @brief Fragmet UV */
	vec3 nearPoint;
	vec3 farPoint;                       
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



/*****************************************************************************************/

/******************************************Functions**************************************/


/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
	float t = - fragInput.nearPoint.y / (fragInput.farPoint.y - fragInput.nearPoint.y);

    outSceneColor = vec4(1.0f, 0.0f, 0.0f, 1.0f * float(t > 0.0f));
}

/*****************************************************************************************/