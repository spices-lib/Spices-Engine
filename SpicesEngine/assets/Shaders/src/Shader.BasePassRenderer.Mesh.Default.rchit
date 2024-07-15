/**
* @file Shader.RayTracingRenderer.RayTracing.Default.rchit.
* @brief This Shader Defines RayTracing Renderer RayTracing SubPass Default Ray Closest Hit Shader Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive  : enable                    /* @brief Enable include Macro.           */
#extension GL_EXT_shader_explicit_arithmetic_types_int64 : require  /* @brief Enable uint64_t type in shader. */

struct MaterialParameter
{
    uint64_t address_11;
};

#include "Header/ShaderClosestHit.glsl"

struct MaterialConstantParameter_11
{
    vec4 diffuseIntensity;
    vec3 normalIntensity;
    vec2 specularIntensity;
    float otherIntensity;
} constParam_11;

layout(buffer_reference, scalar, buffer_reference_align = 8) buffer MaterialConstantParameters_11 { 
    MaterialConstantParameter_11 i[]; 
};

/*****************************************************************************************/


/******************************************Functions**************************************/

void ExplainMaterialParameter(in MaterialParameter param)
{
    uint64_t address = param.address_11;
    MaterialConstantParameters_11 constantParams = MaterialConstantParameters_11(address);
    constParam_11 = constantParams.i[0];
}

void GetMaterialAttributes(in Vertex vt, inout MaterialAttributes attributes)
{  
    attributes.albedo    = vec3(0.5f);
    attributes.roughness = constParam_11.otherIntensity;
    attributes.emissive  = vec3(0.0f);
}

/*****************************************************************************************/