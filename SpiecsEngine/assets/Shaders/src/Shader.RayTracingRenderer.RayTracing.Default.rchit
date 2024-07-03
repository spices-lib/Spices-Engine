/**
* @file Shader.RayTracingRenderer.RayTracing.Default.rchit.
* @brief This Shader Defines RayTracing Renderer RayTracing SubPass Default Ray Closest Hit Shader Behaver.
* @author Spiecs.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive                               : enable
#extension GL_EXT_ray_tracing                                        : require
#extension GL_EXT_nonuniform_qualifier                               : enable
#extension GL_EXT_scalar_block_layout                                : enable
#extension GL_EXT_buffer_reference2                                  : require

#include "Header/ShaderRayCommon.glsl"
#include "Header/ShaderStructures.h"

/*****************************************************************************************/

/********************************Specific Renderer Data***********************************/

layout(location = 0) rayPayloadInEXT HitPayLoad prd;
hitAttributeEXT vec3 attribs;

layout(buffer_reference, scalar) buffer Vertices { Vertex v[]; }; 
layout(buffer_reference, scalar) buffer Indices { ivec3 i[]; };

layout(set = 1, binding = 2, scalar) readonly buffer MeshDescs { MeshDesc i[]; } meshDesc;

layout(push_constant) uniform PushConstant { PushConstantRay push; };

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
    MeshDesc desc       = meshDesc.i[gl_InstanceCustomIndexEXT];
    Vertices vertices   = Vertices(desc.vertexAddress);
    Indices  vertices   = Indices(desc.indexAddress);
    
    prd.hitValue = vec3(1.0f, 1.0f, 0.0f);
}

/*****************************************************************************************/