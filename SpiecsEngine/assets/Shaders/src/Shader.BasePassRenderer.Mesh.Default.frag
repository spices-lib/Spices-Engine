/**
* @file Shader.BasePassRenderer.Mesh.Default.frag.
* @brief This Shader Defines BasePass Renderer Mesh SubPass Default Fragment Shader Behaver.
* @author Spiecs.
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
layout(location = 0) out vec4  outColor;                /* @brief diffuse Attachment.  */
layout(location = 1) out vec4  outNormal;               /* @brief normal Attachment.   */
layout(location = 2) out vec4  outSpecular;             /* @brief specular Attachment. */
layout(location = 3) out vec4  outPosition;             /* @brief position Attachment. */
layout(location = 4) out float outID;                   /* @brief ID Attachment.       */

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
* 0 - Diffuse.
* 1 - Normal.
* 2 - Specular.
*/

#define DIFFUSE 0
#define NORMAL 1
#define SPECULAR 2

/**
* @brief Material Textures.
*/
layout(set = 1, binding = 0) uniform sampler2D samplers[3];

/**
* @brief Material Parameters.
*/
layout(set = 1, binding = 1) uniform Parameter {
    vec4  diffuseIntensity;
    vec3  normalIntensity;
    vec2  specularIntensity;
    float otherIntensity;
};

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
    outColor    = texture(samplers[DIFFUSE], fragInput.texCoord);
    outNormal   = vec4(fragInput.normal * 0.5f + vec3(0.5f), 1.0f);
    outSpecular = texture(samplers[SPECULAR], fragInput.texCoord);
    outPosition = vec4(fragInput.position, 1.0f);
    outID       = push.entityID;
}

/*****************************************************************************************/