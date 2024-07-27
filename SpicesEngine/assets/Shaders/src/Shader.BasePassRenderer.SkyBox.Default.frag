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

/**
* @brief Material Parameter.
* It should be the struct of all textures index and parameter buffer address.
* One index per texture, One address per buffer.
*/
struct MaterialParameter
{
    uint     albedo;
    uint64_t address;
} materialParam;

/**
* @brief Material Constant Parameter.
* It should be the struct of constant parameter buffer data.
*/
struct MaterialConstantParameter{ int empty; } materialConstParam;

#include "Header/ShaderBindLessMaterial.glsl"

/*****************************************************************************************/

/************************************Fragment Input***************************************/

/**
* @brief Fragment Shader Input From Vertex Shader.
*/
layout(location = 0) in struct FragInput {
    vec3 localPosition;
    vec3 worldPosition;
} fragInput;

/*****************************************************************************************/

/************************************Fragment Output**************************************/

/**
* @brief Fragment Shader Output to FrameBuffer.
*/
layout(location = 0) out vec4 outColor;                /* @brief Fragmet Color       */
layout(location = 1) out vec4 outPosition;             /* @brief position Attachment */
layout(location = 2) out float outID;                  /* @brief ID Attachment       */

/*****************************************************************************************/

/*********************************Push Constant*******************************************/

/**
* @brief push constant.
*/
layout(push_constant) uniform Push {
	PushConstantMesh push;                             /* @see PushConstantMesh. */
};

/*****************************************************************************************/

/***************************************Functions*****************************************/

vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(-v.z, v.x), asin(-v.y));
    uv *= invAtan;
    uv += 0.5;

    return uv;
}

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
    vec2 uv = SampleSphericalMap(normalize(fragInput.localPosition)); // make sure to normalize localPos
    outColor = texture(BindLessTextureBuffer[materialParam.albedo], uv);
    outPosition = vec4(fragInput.worldPosition, 1.0f);
    outID = push.desc.entityID;
}

/*****************************************************************************************/