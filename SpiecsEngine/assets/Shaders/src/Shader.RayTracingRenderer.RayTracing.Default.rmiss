/**
* @file Shader.RayTracingRenderer.RayTracing.Default.rmiss.
* @brief This Shader Defines RayTracing Renderer RayTracing SubPass Default Ray Missing Shader Behaver.
* @author Spiecs.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive : enable
#extension GL_EXT_ray_tracing : require

#include "Header/ShaderCommon.h"
#include "Header/ShaderRayCommon.glsl"

/*****************************************************************************************/

/********************************Specific Renderer Data***********************************/

layout(location = 0) rayPayloadInEXT HitPayLoad prd;

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
    prd.hitValue = vec3(0.5f);
}

/*****************************************************************************************/