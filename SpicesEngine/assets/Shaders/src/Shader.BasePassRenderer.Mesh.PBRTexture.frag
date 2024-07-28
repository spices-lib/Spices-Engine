/**
* @file Shader.BasePassRenderer.Mesh.Default.frag.
* @brief This Shader Defines BasePass Renderer Mesh SubPass Default Fragment Shader Behaver.
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
    uint  albedoTexture;
    uint  normalTexture;
    uint  roughnessTexture;
    uint  metallicTexture;
    int   maxRayDepth;
    int   maxLightDepth;
    int   maxShadowDepth;
} materialParam;

#include "Header/ShaderBindLessMaterial.glsl"

/*****************************************************************************************/

/************************************Fragment Input***************************************/

/**
* @brief Fragment Shader Input From Vertex Shader.
*/
layout(location = 0) in struct FragInput {
    vec3 position;                                     /* @brief World Position. */
    vec3 normal;                                       /* @brief World Normal.   */
    vec3 color;                                        /* @brief Fragmet Color.  */
    vec2 texCoord;                                     /* @brief Fragmet UV.     */
} fragInput;

layout(location = 10) in flat uint seed;

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
layout(push_constant) uniform Push {
	PushConstantMesh push;                              /* @see PushConstantMesh. */
};

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
    ExplainMaterialParameter(push.desc.materialParameterAddress);
    
    outAlbedo     = texture(BindLessTextureBuffer[materialParam.albedoTexture], fragInput.texCoord);
    outNormal     = vec4(fragInput.normal * 0.5f + vec3(0.5f), 1.0f);
    outRoughness  = texture(BindLessTextureBuffer[materialParam.roughnessTexture], fragInput.texCoord);
    outMetallic   = texture(BindLessTextureBuffer[materialParam.metallicTexture], fragInput.texCoord);
    outPosition   = vec4(fragInput.position, 1.0f);
    outID         = push.desc.entityID;
}

/*****************************************************************************************/