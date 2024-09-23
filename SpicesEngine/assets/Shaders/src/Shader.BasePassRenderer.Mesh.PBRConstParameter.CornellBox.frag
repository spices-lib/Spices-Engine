/**
* @file Shader.BasePassRenderer.Mesh.PBRConstParameter.CornellBox.frag.
* @brief This Shader Defines BasePass Renderer Mesh SubPass PBRConstParameter CornellBox Fragment Shader Behaver.
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
* @brief Fragment Shader Input From Vertex Shader.
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
layout(location = 0) out vec4  outAlbedo;               /* @brief albedo Attachment.           */
layout(location = 1) out vec4  outNormal;               /* @brief normal Attachment.           */
layout(location = 2) out vec4  outRoughness;            /* @brief roughness Attachment.        */
layout(location = 3) out vec4  outMetallic;             /* @brief metallic Attachment.         */
layout(location = 4) out vec4  outPosition;             /* @brief position Attachment.         */
layout(location = 5) out float outEntityID;             /* @brief EntityID Attachment.         */
layout(location = 6) out vec4  outTriangleID;           /* @brief TriangleID Attachment.       */
layout(location = 7) out vec4  outMeshletID;            /* @brief MeshletID Attachment.        */

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
    
    if(pixel.normal.z > 0.999)
    {
        outAlbedo = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    }
    else if(pixel.normal.z < -0.999)
    {
        outAlbedo = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    }
    else
    {
        outAlbedo = vec4(materialParam.albedo, 1.0f);
    }

    uint  triangleSeed  = triangleId;
    float trianglerand0 = rnd(triangleSeed);
    float trianglerand1 = rnd(triangleSeed);
    float trianglerand2 = rnd(triangleSeed);

    uint  meshletSeed  = meshletId;
    float meshletrand0 = rnd(meshletSeed);
    float meshletrand1 = rnd(meshletSeed);
    float meshletrand2 = rnd(meshletSeed);

    uint  clusterMeshletSeed  = clusterMeshletId;
    float clusterMeshletrand0 = rnd(clusterMeshletSeed);
    float clusterMeshletrand1 = rnd(clusterMeshletSeed);
    float clusterMeshletrand2 = rnd(clusterMeshletSeed);

    outNormal           = vec4(pixel.normal * 0.5f + vec3(0.5f), 1.0f);
    outRoughness        = vec4(materialParam.roughness);
    outMetallic         = vec4(materialParam.metallic);
    outPosition         = vec4(pixel.position, 1.0f);
    outEntityID         = desc.entityID;
    outTriangleID       = vec4(trianglerand0, trianglerand1, trianglerand2, 1.0f);
    outMeshletID        = vec4(meshletrand0, meshletrand1, meshletrand2, 1.0f);
}

/*****************************************************************************************/