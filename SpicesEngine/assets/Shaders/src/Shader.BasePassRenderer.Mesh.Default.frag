/**
* @file Shader.BasePassRenderer.Mesh.Default.frag.
* @brief This Shader Defines BasePass Renderer Mesh SubPass Default Fragment Shader Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive : enable    /* @brief Enable include Macro. */

#include "Header/ShaderCommon.h"

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

/********************************Specific Renderer Data***********************************/

/**
* @brief Textures struct like that:
* 0 - Albedo.
* 1 - Normal.
* 2 - Roughness.
* 3 - Metallic.
*/

#define ALBEDO    0
#define NORMAL    1
#define ROUGHNESS 2
#define METALLIC  3

/**
* @brief Material Textures.
*/
layout(set = 1, binding = 0) uniform sampler2D samplers[4];

/**
* @brief Material Parameters.
*/
layout(set = 1, binding = 1) uniform Parameter {
    vec3  albedo;
    float roughness;
    float metallic;
    int   maxRayDepth;
    int   maxLightDepth;
    int   maxShadowDepth;
};

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
    /*outAlbedo     = texture(samplers[ALBEDO], fragInput.texCoord);
    outNormal     = vec4(fragInput.normal * 0.5f + vec3(0.5f), 1.0f);
    outRoughness  = texture(samplers[ROUGHNESS], fragInput.texCoord);
    outMetallic   = texture(samplers[METALLIC], fragInput.texCoord);
    outPosition   = vec4(fragInput.position, 1.0f);
    outID         = push.entityID;*/
    
    outAlbedo     = vec4(albedo, 1.0f);
    outNormal     = vec4(fragInput.normal * 0.5f + vec3(0.5f), 1.0f);
    outRoughness  = vec4(vec3(clamp(roughness, 0.0f, 1.0f)), 1.0f);
    outMetallic   = vec4(vec3(clamp(metallic, 0.0f, 1.0f)), 1.0f);
    outPosition   = vec4(fragInput.position, 1.0f);
    outID         = push.entityID;
}

/*****************************************************************************************/