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

/**
* @brief Set 1 Binding 1 Buffer Data Struct.
* Both Declear and Instance here.
*/
struct MaterialConstantParameter_11 
{
    vec3 albedo;
    float roughness;
} constParam_11;

/**
* @brief Set 1 Binding 1 Buffer Reference.
* Only Access index 0, though buffer reference no binding check, must be careful work with it.
*/
layout(buffer_reference, scalar, buffer_reference_align = 8) buffer MaterialConstantParameters_11 { 
    MaterialConstantParameter_11 i[]; 
};

/*****************************************************************************************/


/******************************************Functions**************************************/

void ExplainMaterialParameter(in MaterialParameter param)
{
    uint64_t address_11 = param.address_11;
    MaterialConstantParameters_11 constantParams_11 = MaterialConstantParameters_11(address_11);
    constParam_11 = constantParams_11.i[0];
}

void GetMaterialAttributes(in Vertex vt, inout MaterialAttributes attributes)
{  
    attributes.albedo    = constParam_11.albedo;
    attributes.roughness = constParam_11.roughness;
    attributes.emissive  = vec3(0.0f);
}

/*****************************************************************************************/