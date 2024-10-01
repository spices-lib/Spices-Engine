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
	vec3 nearPoint;
	vec3 farPoint;
}
vertOut;

/*****************************************************************************************/

/******************************************Functions**************************************/

/**
* @brief Unproject screen point to world.
* @param[in] x Screen Position X.
* @param[in] y Screen Position Y.
* @param[in] z Depth Z.
* @param[in] view View Matrix.
* @param[in] projection Projection Matrix.
* @return Returns world point.
* @see https://asliceofrendering.com/scene%20helper/2020/01/05/InfiniteGrid/ .
*/
vec3 UnprojectPoint(in float x, in float y, in float z, in mat4 view, in mat4 projection) 
{
    mat4 viewInv                = inverse(view);
    mat4 projInv                = inverse(projection);
    vec4 unprojectedPoint       =  viewInv * projInv * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
	vec2 uv = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
	vertOut.texCoord = uv;

	uv.y = 1.0 - uv.y;
	vec2 p = uv * 2.0f - 1.0f;

	vertOut.nearPoint = UnprojectPoint(p.x, p.y, 1.0f, view.view, view.projection);
	vertOut.farPoint  = UnprojectPoint(p.x, p.y, 0.0f, view.view, view.projection);

	gl_Position = vec4(p, 0.0f, 1.0f);
}

/*****************************************************************************************/