/**
* @file Shader.WorldPickRenderer.WorldPick.Default.frag.
* @brief This Shader Defines WorldPick Renderer WorldPick SubPass Default Fragment Shader Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive : enable   /* @brief Enable include Macro. */

#include "Header/ShaderCommon.h"
#include "Header/ShaderMeshDescLayout.glsl"

/*****************************************************************************************/

/************************************Fragment Input***************************************/

/**
* @brief Fragment Shader Input From Geometry Shader.
*/
layout(location = 0) in struct FragInput 
{
    vec2 texCoord;
} 
fragInput;

/*****************************************************************************************/

/************************************Fragment Output**************************************/

/**
* @brief Fragment Shader Output to FrameBuffer.
*/
layout(location = 0) out float outPick;

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

/**********************************Shader Entry*******************************************/

void main()
{
    ExplainMeshDesciption(push.descAddress);
    outPick = desc.entityID;

    /*float alpha = texture(samplers, fragInput.texCoord).w;
    if (alpha < 0.01f) discard;
    outPick = push.entityID;*/
}

/*****************************************************************************************/