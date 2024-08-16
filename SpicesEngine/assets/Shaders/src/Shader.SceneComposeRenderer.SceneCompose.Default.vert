/**
* @file Shader.SceneComposeRenderer.SceneCompose.Default.vert.
* @brief This Shader Defines SceneCompose Renderer SceneCompose SubPass Default Vertex Shader Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive : enable    /* @brief Enable include Macro. */

#include "Header/ShaderVertexInput.glsl"
#include "Header/ShaderPreRendererLayout.glsl"

/*****************************************************************************************/

/************************************Vertex Output****************************************/

/**
* @brief Vertex Shader Output to Fragment Shader.
*/
layout(location = 0) out struct FragInput
{
    vec2 texCoord;
} 
vertOut;

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
	vertOut.texCoord = texCoord;
	gl_Position = vec4(position * 2.0f, 1.0f);
}

/*****************************************************************************************/