/**
* @file Shader.RayTracingRenderer.RayTracing.Default.rmiss.
* @brief This Shader Defines RayTracing Renderer RayTracing SubPass Default Ray Missing Shader Behaver.
* @author Spiecs.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive : enable    /* @brief Enable include Macro.       */
#extension GL_EXT_ray_tracing          : require   /* @brief Enable Ray Tracing Shader.  */

#include "Header/ShaderCommon.h"

/*****************************************************************************************/

/*********************************Missing Input Output************************************/

/**
* @brief Ray trace payloads.
*/
layout(location = 0) rayPayloadInEXT HitPayLoad prd;

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
    if(prd.rayDirection.y > 0.0f)
    {
        prd.hitValue = mix(vec3(1.0f), vec3(0.25f, 0.5f, 1.0f), prd.rayDirection.y);
    }
    else
    {
        prd.hitValue   = vec3(1.0f);
    }
    prd.depth = 100;                  // Ending trace
}

/*****************************************************************************************/