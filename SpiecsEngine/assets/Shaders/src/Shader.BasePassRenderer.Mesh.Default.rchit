/**
* @file Shader.RayTracingRenderer.RayTracing.Default.rchit.
* @brief This Shader Defines RayTracing Renderer RayTracing SubPass Default Ray Closest Hit Shader Behaver.
* @author Spiecs.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive  : enable    /* @brief Enable include Macro. */

#include "Header/ShaderClosestHit.glsl"

/*****************************************************************************************/

/******************************************Functions**************************************/

void GetMaterialAttributes(in Vertex vt, inout MaterialAttributes attributes)
{  
    attributes.albedo    = vec3(0.5f);
    attributes.roughness = 0.0f;
    attributes.emissive  = vec3(0.0f);
}

/*****************************************************************************************/