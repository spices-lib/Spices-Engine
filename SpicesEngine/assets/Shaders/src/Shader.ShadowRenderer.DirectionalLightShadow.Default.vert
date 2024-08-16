/**
* @file Shader.ShadowRenderer.DirectionalLightShadow.Default.vert.
* @brief This Shader Defines Shadow Renderer DirectionalLightShadow SubPass Default Vertex Shader Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive : enable    /* @brief Enable include Macro. */

#include "Header/ShaderCommon.h"
#include "Header/ShaderVertexInput.glsl"

/*****************************************************************************************/

/*********************************Push Constant*******************************************/

/**
* @brief push constant.
*/
layout(push_constant) uniform Push 
{
	PushConstantMesh push;
};

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main() 
{
	gl_Position = push.model * vec4(position, 1.0f);
}

/*****************************************************************************************/