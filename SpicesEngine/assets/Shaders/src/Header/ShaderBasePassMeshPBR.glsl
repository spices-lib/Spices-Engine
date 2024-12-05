/**
* @file ShaderBasePassMeshPBR.glsl.
* @brief This Shader Header File Defines BasePass Mesh Fragment Shader PBR behave.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#ifndef SHADER_BASEPASS_MESH_PBR
#define SHADER_BASEPASS_MESH_PBR

#extension GL_EXT_fragment_shader_barycentric   : require   /* @brief Enable barycentric access Macro. */

#include "ShaderCommon.h"
#include "ShaderFunctionLibrary.glsl"
#include "ShaderPreRendererLayout.glsl"
#include "ShaderBindLessMaterial.glsl"
#include "ShaderMeshDescLayout.glsl"
#include "ShaderMaterialAttributes.glsl"

/*****************************************************************************************/

/************************************Fragment Input***************************************/

/**
* @brief Fragment Shader Input From Mesh Shader.
*/
layout(location = 0) in flat uint primitiveId;         /* @brief Primitive ID.            */
layout(location = 1) in flat uint meshletId;           /* @brief Meshlet ID.              */
layout(location = 2) in Pixel pixel;                   /* @brief Pixel.                   */

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

/******************************************Functions**************************************/

/**
* @brief Write data to FrameBuffer Gbuffer Attachments.
* @param[in] MaterialAttributes MaterialAttributes.
*/
void WriteDataToGbuffer(const in MaterialAttributes materialAttributes);

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
    /**
    * @brief Access Buffer by GPU address.
    */
    ExplainMeshDesciption(push.descAddress);
    
    MaterialAttributes materialAttributes = InitMaterialAttributes(pixel);                             /* @brief Init material attributes.          */
    GetMaterialAttributes(pixel, materialAttributes);                                                  /* @brief Get material specific attributes.  */
    PostHandleWithMaterialAttributes(materialAttributes);                                              /* @brief Post handle materialAttributes.    */
    
    WriteDataToGbuffer(materialAttributes);
}

/*****************************************************************************************/

void WriteDataToGbuffer(const in MaterialAttributes materialAttributes)
{
    uint  primitiveSeed  = primitiveId;
    float primitiverand0 = rnd(primitiveSeed);
    float primitiverand1 = rnd(primitiveSeed);
    float primitiverand2 = rnd(primitiveSeed);

    uint  meshletSeed  = meshletId;
    float meshletrand0 = rnd(meshletSeed);
    float meshletrand1 = rnd(meshletSeed);
    float meshletrand2 = rnd(meshletSeed);

    outAlbedo           = vec4(materialAttributes.albedo + materialAttributes.emissive, 1.0f);
    outNormal           = vec4(materialAttributes.normal * 0.5f + vec3(0.5f), 1.0f);
    outRoughness        = vec4(materialAttributes.roughness);
    outMetallic         = vec4(materialAttributes.metallic);
    outPosition         = vec4(pixel.position, 1.0f);
    
    outEntityID         = desc.entityID;
    outTriangleID       = vec4(primitiverand0, primitiverand1, primitiverand2, 1.0f);
    outMeshletID        = vec4(meshletrand0, meshletrand1, meshletrand2, 1.0f);
}

void ReverseBackFaceNormal(inout MaterialAttributes attr)
{
    vec4 ro = view.inView * vec4(0.0f, 0.0f, 0.0f, 1.0f);
    vec3 ra = normalize(pixel.position - ro.xyz);

    if(dot(-ra, attr.normal) < 0.0f)
    {
        attr.normal *= -1.0f;
    }
}

#endif