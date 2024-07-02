/**
* @file Shader.RayTracingRenderer.RayTracing.Default.rmiss.
* @brief This Shader Defines RayTracing Renderer RayTracing SubPass Default Ray Missing Shader Behaver.
* @author Spiecs.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive : enable
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_shader_explicit_arithmetic_types_int64 : require

#include "Header/ShaderRayCommon.glsl"

/*****************************************************************************************/

/********************************Specific Renderer Data***********************************/

layout(location = 0) rayPayloadInEXT HitPayLoad prd;

//layout(push_constant) uniform _PushConstantRay {
//    PushConstantRay pcRay;
//}

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
    prd.hitValue = vec3(1.0f) * 0.8f;
}

/*****************************************************************************************/