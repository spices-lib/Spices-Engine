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
layout(location = 0) in Pixel pixel;                   /* @brief Pixel Data.              */
layout(location = 4) in flat uint triangleId;          /* @brief Triangle ID.             */
layout(location = 5) in flat uint meshletId;           /* @brief Meshlet ID.              */
layout(location = 6) in flat uint clusterMeshletId;    /* @brief ClusterMeshletId ID.     */

/*****************************************************************************************/

/************************************Fragment Output**************************************/

/**
* @brief Fragment Shader Output to FrameBuffer.
*/
layout(location = 0) out vec4  outAR;                   /* @brief albedo roughnss Attachment.   */
layout(location = 1) out vec4  outNM;                    /* @brief normal metallic Attachment.  */
layout(location = 2) out vec4  outPosition;             /* @brief position Attachment.          */
layout(location = 3) out uvec4 outETMID;                /* @brief EntityID Attachment.          */

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

    //uint  triangleSeed  = triangleId;
    //float trianglerand0 = rnd(triangleSeed);
    //float trianglerand1 = rnd(triangleSeed);
    //float trianglerand2 = rnd(triangleSeed);

    //uint  meshletSeed  = meshletId;
    //float meshletrand0 = rnd(meshletSeed);
    //float meshletrand1 = rnd(meshletSeed);
    //float meshletrand2 = rnd(meshletSeed);

    //uint  clusterMeshletSeed  = clusterMeshletId;
    //float clusterMeshletrand0 = rnd(clusterMeshletSeed);
    //float clusterMeshletrand1 = rnd(clusterMeshletSeed);
    //float clusterMeshletrand2 = rnd(clusterMeshletSeed);

    outAR               = vec4(materialParam.albedo, materialParam.roughness);
    vec3 normal         = vec3(pixel.normal * 0.5f + vec3(0.5f));
    outNM               = vec4(normal.xyz, materialParam.metallic);
    outPosition         = vec4(pixel.position, 0.0f);
    outETMID            = uvec4(desc.entityID.x, triangleId, meshletId, 0);
}

/*****************************************************************************************/