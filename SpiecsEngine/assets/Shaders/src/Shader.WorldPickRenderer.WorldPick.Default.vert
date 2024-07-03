/**
* @file Shader.WorldPickRenderer.WorldPick.Default.vert.
* @brief This Shader Defines WorldPick Renderer WorldPick SubPass Default Vertex Shader Behaver.
* @author Spiecs.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive : enable

#include "Header/ShaderVertexInput.glsl"
#include "Header/ShaderLayouts.glsl"

/*****************************************************************************************/

/************************************Vertex Output****************************************/

/**
* @brief Vertex Shader Output to Fragment Shader.
*/
layout(location = 0) out struct FragInput {
    vec2 texCoord;
} vertOut;

/*****************************************************************************************/

/*********************************Push Constant*******************************************/

/**
* @brief push constant.
*/
layout(push_constant) uniform Push {
	PushConstantMesh push;
};

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
    vertOut.texCoord = texCoord;
    gl_Position = view.projection * view.view * push.model * vec4(position, 1.0);
}

/*****************************************************************************************/