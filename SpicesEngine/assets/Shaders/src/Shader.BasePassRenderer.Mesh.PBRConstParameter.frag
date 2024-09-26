/**
* @file Shader.BasePassRenderer.Mesh.PBRConstParameter.frag.
* @brief This Shader Defines BasePass Renderer Mesh SubPass PBRConstParameter Fragment Shader Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive          : enable    /* @brief Enable include Macro.            */
#extension GL_EXT_fragment_shader_barycentric   : require   /* @brief Enable barycentric access Macro. */

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
layout(location = 0) in flat uint primitiveId;         /* @brief Primitive ID.            */
layout(location = 1) in flat uint meshletId;           /* @brief Meshlet ID.              */

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
    Pixel pixel = GetPixelUsingPrimitiveBarycentric(primitiveId, gl_BaryCoordEXT);

    uint  primitiveSeed  = primitiveId;
    float primitiverand0 = rnd(primitiveSeed);
    float primitiverand1 = rnd(primitiveSeed);
    float primitiverand2 = rnd(primitiveSeed);

    uint  meshletSeed  = meshletId;
    float meshletrand0 = rnd(meshletSeed);
    float meshletrand1 = rnd(meshletSeed);
    float meshletrand2 = rnd(meshletSeed);

    outAlbedo           = vec4(materialParam.albedo, 1.0f);
    outNormal           = vec4(pixel.normal * 0.5f + vec3(0.5f), 1.0f);
    outRoughness        = vec4(materialParam.roughness);
    outMetallic         = vec4(materialParam.metallic);
    outPosition         = vec4(pixel.position, 1.0f);
    outEntityID         = desc.entityID;
    outTriangleID       = vec4(primitiverand0, primitiverand1, primitiverand2, 1.0f);
    outMeshletID        = vec4(meshletrand0, meshletrand1, meshletrand2, 1.0f);
}

/*****************************************************************************************/