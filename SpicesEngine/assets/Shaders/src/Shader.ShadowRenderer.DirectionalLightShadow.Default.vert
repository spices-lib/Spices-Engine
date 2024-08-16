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
    MeshDesc desc;                           /* @see MeshDesc. */
};

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
	mat4 model  = Model(desc.modelAddredd).i[0];
	gl_Position = model * vec4(position, 1.0f);
}

/*****************************************************************************************/