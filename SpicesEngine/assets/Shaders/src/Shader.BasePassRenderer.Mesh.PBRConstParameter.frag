/**
* @file Shader.BasePassRenderer.Mesh.PBRConstParameter.frag.
* @brief This Shader Defines BasePass Renderer Mesh SubPass PBRConstParameter Fragment Shader Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive : enable    /* @brief Enable include Macro. */

#include "Header/ShaderCommon.h"
#include "Header/ShaderPreRendererLayout.glsl"
#include "Header/ShaderFunctionLibrary.glsl"

/**
* @brief Material Parameter.
* It should be the struct of all textures index and parameter buffer address.
* One index per texture, One address per buffer.
*/
struct MaterialParameter
{
    vec3  albedo;
    float roughness;
    float metallic;
    int   maxRayDepth;
    int   maxLightDepth;
    int   maxShadowDepth;
};

#include "Header/ShaderBindLessMaterial.glsl"
#include "Header/ShaderMeshDescLayout.glsl"

/*****************************************************************************************/

/************************************Fragment Input***************************************/

/**
* @brief Fragment Shader Input From Mesh Shader.
*/
layout(location = 0) in Pixel pixel;                   /* @brief Pixel Data.     */
layout(location = 4) in flat uint meshletId;           /* @brief Meshlet ID.     */

/*****************************************************************************************/

/************************************Fragment Output**************************************/

/**
* @brief Fragment Shader Output to FrameBuffer.
*/
layout(location = 0) out vec4  outAlbedo;               /* @brief albedo Attachment.   */
layout(location = 1) out vec4  outNormal;               /* @brief normal Attachment.   */
layout(location = 2) out vec4  outRoughness;            /* @brief roughness Attachment.*/
layout(location = 3) out vec4  outMetallic;             /* @brief metallic Attachment. */
layout(location = 4) out vec4  outPosition;             /* @brief position Attachment. */
layout(location = 5) out float outID;                   /* @brief ID Attachment.       */

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

    //outAlbedo    = vec4(materialParam.albedo, 1.0f);

    uint  seed0 = meshletId;
    float rand0 = rnd(seed0);
    float rand1 = rnd(seed0);
    float rand2 = rnd(seed0);
    outAlbedo = vec4(rand0, rand1, rand2, 1.0f);

    outNormal    = vec4(pixel.normal * 0.5f + vec3(0.5f), 1.0f);
    outRoughness = vec4(materialParam.roughness);
    outMetallic  = vec4(materialParam.metallic);
    outPosition  = vec4(pixel.position, 1.0f);
    outID        = desc.entityID;
}

/*****************************************************************************************/