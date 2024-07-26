/**
* @file Shader.RayTracingRenderer.RayTracing.interior_stair_wl3ieamdw.rchit.
* @brief This Shader Defines RayTracing Renderer RayTracing SubPass interior_stair_wl3ieamdw Ray Closest Hit Shader Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive  : enable                    /* @brief Enable include Macro.           */
#extension GL_EXT_shader_explicit_arithmetic_types_int64 : require  /* @brief Enable uint64_t type in shader. */

/**
* @brief Material Parameter.
* It should be the struct of all textures index and parameter buffer address.
* One index per texture, One address per buffer.
*/
struct MaterialParameter
{
    uint albedoTexture;
    uint normalTexture;
    uint roughnessTexture;
    uint metallicTexture;
    int  maxRayDepth;
    int  maxLightDepth;
    int  maxShadowDepth;
} materialParam;

/**
* @brief Closest Hit Shader Entry Point.
*/
#include "Header/ShaderClosestHitPBR.glsl"

/*****************************************************************************************/

/******************************************Functions**************************************/

void GetMaterialAttributes(in Pixel pi, inout MaterialAttributes attributes)
{
    attributes.albedo          = texture(BindLessTextureBuffer[materialParam.albedoTexture],    pi.texCoord).xyz;
    attributes.roughness       = texture(BindLessTextureBuffer[materialParam.roughnessTexture], pi.texCoord).x;
    //attributes.metallic        = texture(BindLessTextureBuffer[materialParam.metallicTexture],  pi.texCoord).x;
    attributes.emissive        = vec3(0.0f);
    attributes.maxRayDepth     = materialParam.maxRayDepth;
    attributes.maxLightDepth   = materialParam.maxLightDepth;
    attributes.maxShadowDepth  = materialParam.maxShadowDepth;
}

/*****************************************************************************************/