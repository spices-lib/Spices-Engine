/**
* @file Shader.RayTracingRenderer.RayTracing.Default.rchit.
* @brief This Shader Defines RayTracing Renderer RayTracing SubPass Default Ray Closest Hit Shader Behaver.
* @author Spiecs.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive : enable
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_shader_explicit_arithmetic_types_int64 : require
#extension GL_EXT_buffer_reference2 : require

#include "Header/ShaderRayCommon.glsl"

/*****************************************************************************************/

/********************************Specific Renderer Data***********************************/

layout(location = 0) rayPayloadInEXT HitPayLoad prd;

//layout(buffer_reference, scalar) buffer Vertices {Vertex v[]; }; // Positions of an object
//layout(buffer_reference, scalar) buffer Indices {ivec3 i[]; }; // Triangle indices
//layout(buffer_reference, scalar) buffer Materials {WaveFrontMaterial m[]; }; // Array of all materials on an object
//layout(buffer_reference, scalar) buffer MatIndices {int i[]; }; // Material ID for each triangle
//layout(set = 1, binding = eObjDescs, scalar) buffer ObjDesc_ { ObjDesc i[]; } objDesc;

hitAttributeEXT vec3 attribs;

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
  prd.hitValue = vec3(0.2, 0.5, 0.5);
}

/*****************************************************************************************/