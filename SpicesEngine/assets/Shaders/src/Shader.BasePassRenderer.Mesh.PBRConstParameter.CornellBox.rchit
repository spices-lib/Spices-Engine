/**
* @file Shader.RayTracingRenderer.RayTracing.Default.rchit.
* @brief This Shader Defines RayTracing Renderer RayTracing SubPass Default Ray Closest Hit Shader Behaver.
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
    vec3  albedo;
    float roughness;
    float metallic;
    int   maxRayDepth;
    int   maxLightDepth;
    int   maxShadowDepth;
} materialParam;

/**
* @brief Closest Hit Shader Entry Point.
*/
#include "Header/ShaderClosestHitPBR.glsl"

/*****************************************************************************************/

/******************************************Functions**************************************/

void GetMaterialAttributes(in Pixel pi, inout MaterialAttributes attributes)
{
    if(pi.normal.z > 0.999)
    {
        attributes.albedo      = vec3(1.0f, 0.0f, 0.0f);
    }
    else if(pi.normal.z < -0.999)
    {
        attributes.albedo      = vec3(0.0f, 1.0f, 0.0f);
    }
    else
    {
        attributes.albedo = materialParam.albedo;
    }
    attributes.roughness       = materialParam.roughness;
    attributes.metallic        = materialParam.metallic;
    attributes.emissive        = vec3(0.0f);
    attributes.maxRayDepth     = materialParam.maxRayDepth;
    attributes.maxLightDepth   = materialParam.maxLightDepth;
    attributes.maxShadowDepth  = materialParam.maxShadowDepth;
}

/*****************************************************************************************/