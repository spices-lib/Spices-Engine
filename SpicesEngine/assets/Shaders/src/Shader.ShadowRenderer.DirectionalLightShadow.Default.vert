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
#include "Header/ShaderMeshDescLayout.glsl"

/*****************************************************************************************/

/*********************************Push Constant*******************************************/

/**
* @brief push constant.
*/
layout(push_constant) uniform Push 
{
    uint64_t descAddress;
} 
push;

/*****************************************************************************************/

/************************************Specific Data****************************************/

layout(buffer_reference, scalar, buffer_reference_align = 8) buffer Model
{
    mat4 i[];
};

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main() 
{
    ExplainMeshDesciption(push.descAddress);
	gl_Position = model * vec4(position, 1.0f);
}

/*****************************************************************************************/