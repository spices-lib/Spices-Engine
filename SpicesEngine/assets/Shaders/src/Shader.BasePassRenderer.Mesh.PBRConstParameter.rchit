/**
* @file Shader.BasePassRenderer.Mesh.PBRConstParameter.rchit.
* @brief This Shader Defines RayTracing Renderer RayTracing SubPass PBRConstParameter Ray Closest Hit Shader Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive  : enable                    /* @brief Enable include Macro.           */

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
    attributes.albedo          = materialParam.albedo;
    attributes.roughness       = materialParam.roughness;
    attributes.metallic        = materialParam.metallic;
    attributes.emissive        = vec3(0.0f);
    attributes.maxRayDepth     = max(materialParam.maxRayDepth, 0);
    attributes.maxLightDepth   = max(materialParam.maxLightDepth, 0);
    attributes.maxShadowDepth  = max(materialParam.maxShadowDepth, 0);
}

/*****************************************************************************************/