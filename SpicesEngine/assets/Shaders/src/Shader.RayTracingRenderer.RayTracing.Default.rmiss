/**
* @file Shader.RayTracingRenderer.RayTracing.Default.rmiss.
* @brief This Shader Defines RayTracing Renderer RayTracing SubPass Default Ray Missing Shader Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive : enable    /* @brief Enable include Macro.       */
#extension GL_EXT_ray_tracing          : require   /* @brief Enable Ray Tracing Shader.  */

#include "Header/ShaderCommon.h"
#include "Header/ShaderFunctionLibrary.glsl"

/*****************************************************************************************/

/*********************************Missing Input Output************************************/

/**
* @brief Ray trace payloads.
*/
layout(location = 0) rayPayloadInEXT HitPayLoad prd;

/*****************************************************************************************/

layout(set = 3, binding = 0) uniform sampler2D samplers;
layout(push_constant) uniform Push {
    int entityID;
} push;

/**********************************Shader Entry*******************************************/

void main()
{
    if(prd.rayDirection.y > 0.0f)
    {
        //prd.hitValue = mix(vec3(1.0f), vec3(0.25f, 0.5f, 1.0f), prd.rayDirection.y);
    }
    else
    {
        //prd.hitValue   = vec3(1.0f);
    }
    vec2 uv = SampleSphericalMap(normalize(prd.rayDirection));
    prd.hitValue      = texture(samplers, uv).xyz;
    prd.rayDepth      = 100;                  // Ending trace
    prd.maxRayDepth   = 0;
    prd.entityID      = push.entityID;
}

/*****************************************************************************************/