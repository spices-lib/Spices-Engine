/**
* @file Shader.BasePassRenderer.SkyBox.Default.frag.
* @brief This Shader Defines BasePass Renderer SkyBox SubPass Default Fragment Shader Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive : enable        /* @brief Enable include Macro. */

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
    uint     albedo;
};

#include "Header/ShaderBindLessMaterial.glsl"
#include "Header/ShaderMeshDescLayout.glsl"

/*****************************************************************************************/

/************************************Fragment Input***************************************/

/**
* @brief Fragment Shader Input From Mesh Shader.
*/
layout(location = 0) in struct FragInput 
{
    vec3 localPosition;
    vec3 worldPosition;
} 
fragInput;

layout(location = 2) in flat uint triangleId;          /* @brief Triangle ID.             */
layout(location = 3) in flat uint meshletId;           /* @brief Meshlet ID.              */
layout(location = 4) in flat uint clusterMeshletId;    /* @brief ClusterMeshletId ID.     */

/*****************************************************************************************/

/************************************Fragment Output**************************************/

/**
* @brief Fragment Shader Output to FrameBuffer.
*/
layout(location = 0) out vec4  outColor;                /* @brief Fragmet Color                */
layout(location = 1) out vec4  outPosition;             /* @brief position Attachment          */
layout(location = 2) out float outID;                   /* @brief EntityID Attachment          */
layout(location = 3) out vec4  outTriangleID;           /* @brief TriangleID Attachment.       */
layout(location = 4) out vec4  outMeshletID;            /* @brief MeshletID Attachment.        */

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

    vec2 uv             = SampleSphericalMap(normalize(fragInput.localPosition)); // make sure to normalize localPos
    outColor            = texture(BindLessTextureBuffer[materialParam.albedo], uv);
    outPosition         = vec4(fragInput.worldPosition, 1.0f);
    outID               = desc.entityID;
    outTriangleID       = vec4(trianglerand0, trianglerand1, trianglerand2, 1.0f);
    outMeshletID        = vec4(meshletrand0, meshletrand1, meshletrand2, 1.0f);
}

/*****************************************************************************************/