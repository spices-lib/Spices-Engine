/**
* @file Shader.SpriteRenderer.Sprite.Default.frag.
* @brief This Shader Defines Sprite Renderer Sprite SubPass Default Fragment Shader Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive : enable    /* @brief Enable include Macro. */

#include "Header/ShaderCommon.h"
#include "Header/ShaderPreRendererLayout.glsl"

/**
* @brief Material Parameter.
* It should be the struct of all textures index and parameter buffer address.
* One index per texture, One address per buffer.
*/
struct MaterialParameter
{
    uint  albedoTexture;
} 
materialParam;

#include "Header/ShaderBindlessMaterial.glsl"

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
layout(location = 0) out vec4 outSceneColor;
layout(location = 1) out float outID;

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

/**********************************Shader Entry*******************************************/

void main()
{
    ExplainMaterialParameter(desc.materialParameterAddress);
    
    vec4 texColor = texture(BindLessTextureBuffer[materialParam.albedoTexture], fragInput.texCoord);

    if (texColor.w < 0.01f) discard;

    outSceneColor = texColor;
    outID = desc.entityID;
}

/*****************************************************************************************/