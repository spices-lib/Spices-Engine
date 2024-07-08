/**
* @file Shader.BasePassRenderer.SkyBox.Default.vert.
* @brief This Shader Defines BasePass Renderer SkyBox SubPass Default Vertex Shader Behaver.
* @author Spiecs.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive : enable     /* @brief Enable include Macro. */

#include "Header/ShaderVertexInput.glsl"
#include "Header/ShaderPreRendererLayout.glsl"

/*****************************************************************************************/

/************************************Vertex Output****************************************/

/**
* @brief Vertex Shader Output to Fragment Shader.
*/
layout(location = 0) out struct FragInput {
    vec3 localPosition;
    vec3 worldPosition;
} vertOut;

/*****************************************************************************************/

/*********************************Push Constant*******************************************/

/**
* @brief push constant.
*/
layout(push_constant) uniform Push {
	PushConstantMesh push;                          /* @see PushConstantMesh. */
};

/*****************************************************************************************/


/**********************************Shader Entry*******************************************/

void main()
{
    vec4 pos = push.model * vec4(position, 1.0);

    vertOut.localPosition = position;
    vertOut.worldPosition = pos.xyz;

    gl_Position = view.projection * view.view * pos;
}

/*****************************************************************************************/