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
* One index per texture, One address per buffer set binding.
*/
struct MaterialParameter
{
    uint64_t address_11;         /* @brief Address of Set 1 Binding 1 Buffer. */
};

/**
* @brief Closest Hit Shader Entry Point.
*/
#include "Header/ShaderClosestHit.glsl"

/*****************************************************************************************/

/********************************Specific Material Data***********************************/

BEGIN_DECLEAR_CONST_PARAM(11)
vec3  albedo;
float roughness;
float metallic;
int   maxRayDepth;
int   maxLightDepth;
int   maxShadowDepth;
END_DECLEAR_CONST_PARAM(11)

/*****************************************************************************************/

/******************************************Functions**************************************/

void ExplainMaterialParameter(in MaterialParameter param)
{
    EXPLAIN_CONST_PARAM(11)
}

void GetMaterialAttributes(in Pixel pi, inout MaterialAttributes attributes)
{
    if(pi.normal.z > 0.999f)
    {
        attributes.albedo      = vec3(1.0f, 0.0f, 0.0f);
    }
    else if(pi.normal.z < -0.999f)
    {
        attributes.albedo      = vec3(0.0f, 1.0f, 0.0f);
    }
    else
    {
        attributes.albedo      = CONST_PARAM(11).albedo;
    }
    attributes.roughness       = CONST_PARAM(11).roughness;
    attributes.metallic        = CONST_PARAM(11).metallic;
    attributes.emissive        = vec3(0.0f);
    attributes.maxRayDepth     = CONST_PARAM(11).maxRayDepth;
    attributes.maxLightDepth   = CONST_PARAM(11).maxLightDepth;
    attributes.maxShadowDepth  = CONST_PARAM(11).maxShadowDepth;
}

/*****************************************************************************************/