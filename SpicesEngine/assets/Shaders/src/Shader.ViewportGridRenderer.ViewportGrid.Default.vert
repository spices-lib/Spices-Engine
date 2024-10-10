/**
* @file Shader.ViewportGridRenderer.ViewportGrid.Default.vert.
* @brief This Shader Defines ViewportGrid Renderer ViewportGrid SubPass Default Vertex Shader Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive : enable     /* @brief Enable include Macro. */

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
	vec2 uv           = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
	vertOut.texCoord  = uv;

	uv.y   = 1.0 - uv.y;
	gl_Position = vec4(uv * 2.0f - 1.0f, 1.0f, 1.0f);
}

/*****************************************************************************************/