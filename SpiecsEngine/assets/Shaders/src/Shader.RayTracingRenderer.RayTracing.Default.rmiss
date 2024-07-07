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

/*****************************************************************************************/

/********************************Specific Renderer Data***********************************/

layout(location = 0) rayPayloadInEXT HitPayLoad prd;

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
    if(prd.depth == 0)
    {
        prd.hitValue = vec3(0.3f);
    }
    else
    {
        prd.hitValue = vec3(0.01f);    // No contribution from environment
    }
    prd.depth = 100;                  // Ending trace
}

/*****************************************************************************************/