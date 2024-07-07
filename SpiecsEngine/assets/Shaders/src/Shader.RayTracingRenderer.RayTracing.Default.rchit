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

#include "Header/ShaderCommon.h"

/*****************************************************************************************/

/********************************Specific Renderer Data***********************************/

layout(location = 0) rayPayloadInEXT HitPayLoad prd;
hitAttributeEXT vec3 attribs;

layout(buffer_reference, scalar, buffer_reference_align = 8) buffer Vertices { Vertex v[]; }; 
layout(buffer_reference, scalar, buffer_reference_align = 8) buffer Indices { ivec3 i[]; };

layout(set = 1, binding = 0) uniform accelerationStructureEXT topLevelAS;
layout(set = 1, binding = 2, scalar) readonly buffer MeshDescs { MeshDesc i[]; } meshDesc;

layout(push_constant) uniform PushConstant { PushConstantRay push; };

layout(location = 1) rayPayloadEXT bool isShadowed;

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
    MeshDesc desc       = meshDesc.i[gl_InstanceCustomIndexEXT];
    Vertices vertices   = Vertices(desc.vertexAddress);
    Indices  indices    = Indices(desc.indexAddress);
    
    // Indices of the triangle
    ivec3 index = indices.i[gl_PrimitiveID];
    
    // Vertex of the triangle
    Vertex v0 = vertices.v[index.x];
    Vertex v1 = vertices.v[index.y];
    Vertex v2 = vertices.v[index.z];
    
    const vec3 barycentrics = vec3(1.0 - attribs.x - attribs.y, attribs.x, attribs.y);
    
    //vec3 worldPos = gl_WorldRayOriginEXT + gl_WorldRayDirectionEXT * gl_HitTEXT;
    
    // Computing the coordinates of the hit position
    const vec3 pos      = v0.position * barycentrics.x + v1.position * barycentrics.y + v2.position * barycentrics.z;
    const vec3 worldPos = vec3(gl_ObjectToWorldEXT * vec4(pos, 1.0));  // Transforming the position to world space
    
    // Computing the normal at hit position
    const vec3 nrm      = v0.normal * barycentrics.x + v1.normal * barycentrics.y + v2.normal * barycentrics.z;
    const vec3 worldNrm = normalize(vec3(nrm * gl_WorldToObjectEXT));  // Transforming the normal to world space
    
    // Vector toward the light
    vec3  L;
    float lightIntensity = push.lightIntensity;
    float lightDistance  = 100000.0;
    
    // Point light
    if(push.lightType == 0)
    {
        vec3 lDir      = push.lightPosition - worldPos;
        lightDistance  = length(lDir);
        lightIntensity = push.lightIntensity / (lightDistance * lightDistance);
        L              = normalize(lDir);
    }
    else  // Directional light
    {
        L = normalize(push.lightPosition);
    }
    
    vec3 col = dot(vec3(1.0f), worldNrm).xxx;
    if(dot(worldNrm, L) > 0)
    {
        float tMin = 0.001;
        float tMax = lightDistance;
        vec3 origin = gl_WorldRayOriginEXT + gl_WorldRayDirectionEXT * gl_HitTEXT;
        vec3 rayDir = L;
        uint flags = gl_RayFlagsTerminateOnFirstHitEXT | gl_RayFlagsOpaqueEXT | gl_RayFlagsSkipClosestHitShaderEXT;
        isShadowed = true;
        traceRayEXT(topLevelAS,  // acceleration structure
            flags,               // rayFlags
            0xFF,                // cullMask
            0,                   // sbtRecordOffset
            0,                   // sbtRecordStride
            1,                   // missIndex
            origin,              // ray origin
            tMin,                // ray min range
            rayDir,              // ray direction
            tMax,                // ray max range
            1                    // payload (location = 1)
        );
        
        if(isShadowed)
        {
            col = vec3(0.0f);
        }
        else
        {
        }
    }
    
    prd.hitValue = col;
}

/*****************************************************************************************/