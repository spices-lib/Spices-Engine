/**
* @file Shader.BasePassRenderer.SkyBox.Default.frag.
* @brief This Shader Defines BasePass Renderer SkyBox SubPass Default Fragment Shader Behaver.
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
    uint     albedo;
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

layout(location = 2) in pervertexEXT struct FragInput
{
    vec3 localPosition;
    vec3 worldPosition;
}
fragInputs[];
FragInput fragInput;

/*****************************************************************************************/

/************************************Fragment Output**************************************/

/**
* @brief Fragment Shader Output to FrameBuffer.
*/
layout(location = 0) out vec4  outColor;                /* @brief Fragmet Color                */
layout(location = 1) out vec4  outPosition;             /* @brief position Attachment          */
layout(location = 2) out float outEntityID;             /* @brief EntityID Attachment          */
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
    Pixel pixel = GetPixelUsingPrimitiveBarycentric(primitiveId, gl_BaryCoordEXT);

    fragInput.localPosition = fragInputs[0].localPosition * gl_BaryCoordEXT.x + fragInputs[1].localPosition * gl_BaryCoordEXT.y + fragInputs[2].localPosition * gl_BaryCoordEXT.z;
    fragInput.worldPosition = fragInputs[0].worldPosition * gl_BaryCoordEXT.x + fragInputs[1].worldPosition * gl_BaryCoordEXT.y + fragInputs[2].worldPosition * gl_BaryCoordEXT.z;

    uint  primitiveSeed  = primitiveId;
    float primitiverand0 = rnd(primitiveSeed);
    float primitiverand1 = rnd(primitiveSeed);
    float primitiverand2 = rnd(primitiveSeed);

    uint  meshletSeed  = meshletId;
    float meshletrand0 = rnd(meshletSeed);
    float meshletrand1 = rnd(meshletSeed);
    float meshletrand2 = rnd(meshletSeed);

    vec2 uv             = SampleSphericalMap(normalize(fragInput.localPosition)); // make sure to normalize localPos
    outColor            = texture(BindLessTextureBuffer[materialParam.albedo], uv);
    outPosition         = vec4(fragInput.worldPosition, 1.0f);
    outEntityID         = desc.entityID;
    outTriangleID       = vec4(primitiverand0, primitiverand1, primitiverand2, 1.0f);
    outMeshletID        = vec4(meshletrand0, meshletrand1, meshletrand2, 1.0f);
}

/*****************************************************************************************/