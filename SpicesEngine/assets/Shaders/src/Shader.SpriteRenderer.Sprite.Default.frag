/**
* @file Shader.SpriteRenderer.Sprite.Default.frag.
* @brief This Shader Defines Sprite Renderer Sprite SubPass Default Fragment Shader Behaver.
* @author Spiecs.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive : enable    /* @brief Enable include Macro. */

#include "Header/ShaderCommon.h"

/*****************************************************************************************/

/************************************Fragment Input***************************************/

/**
* @brief Fragment Shader Input From Geometry Shader.
*/
layout(location = 0) in struct FragInput {
    vec2 texCoord;
} fragInput;

/*****************************************************************************************/

/************************************Fragment Output**************************************/

/**
* @brief Fragment Shader Output to FrameBuffer.
*/
layout(location = 0) out vec4 outSceneColor;
layout(location = 1) out float outID;

/*****************************************************************************************/

/*********************************Push Constant*******************************************/

/**
* @brief push constant.
*/
layout(push_constant) uniform Push {
	PushConstantMesh push;
};

/*****************************************************************************************/

/********************************Specific Renderer Data***********************************/

/**
* @brief Sprite Texture.
*/
layout(set = 1, binding = 0) uniform sampler2D samplers;

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
    vec4 texColor = texture(samplers, fragInput.texCoord);

    if (texColor.w < 0.01f) discard;

    outSceneColor = texColor;
    outID = push.entityID;
}

/*****************************************************************************************/