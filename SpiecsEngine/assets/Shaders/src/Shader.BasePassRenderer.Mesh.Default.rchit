/**
* @file Shader.RayTracingRenderer.RayTracing.Default.rchit.
* @brief This Shader Defines RayTracing Renderer RayTracing SubPass Default Ray Closest Hit Shader Behaver.
* @author Spiecs.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive  : enable    /* @brief Enable include Macro.                */
#extension GL_EXT_ray_tracing           : require   /* @brief Enable Ray Tracing Shader.           */
#extension GL_EXT_nonuniform_qualifier  : enable    /* @brief Enable Bindless DescriptorSet.       */
#extension GL_EXT_buffer_reference2     : require   /* @brief Enable Shader Buffer Address access. */

#include "Header/ShaderCommon.h"
#include "Header/ShaderFunctionLibrary.glsl"

/*****************************************************************************************/

/**********************************Closest Hit Input**************************************/

/** 
* @brief Hit Triangle Center Weight. 
*/
hitAttributeEXT vec3 attribs;

/**
* @brief True if is shadow area, data from Miss Shader.
*/
layout(location = 1) rayPayloadEXT bool isShadowed;

/*****************************************************************************************/

/*******************************Closest Hit Input Output**********************************/

/**
* @brief Ray trace payloads.
*/
layout(location = 0) rayPayloadInEXT HitPayLoad prd;

/*****************************************************************************************/

/********************************Specific Renderer Data***********************************/

/**
* @brief Buffer of all Vertices in World.
*/
layout(buffer_reference, scalar, buffer_reference_align = 8) buffer Vertices { 
    Vertex v[];             /* @see Vertex. */
};

/**
* @brief Buffer of all Indices in World.
*/
layout(buffer_reference, scalar, buffer_reference_align = 8) buffer Indices { 
    ivec3 i[]; 
};

/**
* @brief Acceleration Structure.
*/
layout(set = 1, binding = 0) uniform accelerationStructureEXT topLevelAS;

/**
* @brief MeshDescription Buffer of all Mesh in World.
*/
layout(set = 1, binding = 2, scalar) readonly buffer MeshDescBuffer { 
    MeshDesc i[];           /* @see MeshDesc. */
} meshDescBuffer;

/**
* @brief DirectionalLight Buffer in World.
*/
layout(set = 1, binding = 3, scalar) readonly buffer DLightBuffer   { 
    DirectionalLight i[];   /* @see DirectionalLight. */
} dLightBuffer;

/**
* @brief PointLight Buffer in World.
*/
layout(set = 1, binding = 4, scalar) readonly buffer PLightBuffer   { 
    PointLight i[];         /* @see PointLight. */
} pLightBuffer;

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
    MeshDesc desc       = meshDescBuffer.i[gl_InstanceCustomIndexEXT];
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
    
    // light position
    //vec3 lpos = pLightBuffer.i[0].position;
    //vec3 dir = normalize(lpos - worldPos);
    
    //vec3 col = dot(worldNrm, dir) * pLightBuffer.i[0].color;

    //if(dot(worldNrm, dir) > 0)
    //{
    //    float tMin = 0.001f;
    //    float tMax = length(lpos - worldPos);
    //    vec3 origin = worldPos;
    //    vec3 rayDir = dir;
    //    uint flags = gl_RayFlagsTerminateOnFirstHitEXT | gl_RayFlagsOpaqueEXT | gl_RayFlagsSkipClosestHitShaderEXT;
    //    isShadowed = true;
    //    traceRayEXT(topLevelAS,  // acceleration structure
    //        flags,               // rayFlags
    //        0xFF,                // cullMask
    //        0,                   // sbtRecordOffset
    //        0,                   // sbtRecordStride
    //        1,                   // missIndex
    //        origin,              // ray origin
    //        tMin,                // ray min range
    //        rayDir,              // ray direction
    //        tMax,                // ray max range
    //        1                    // payload (location = 1)
    //    );
    //    
    //    if(isShadowed)
    //    {
    //        col = vec3(0.0f);
    //    }
    //    else
    //    {
    //    }
    //}
    
    //prd.hitValue = col;


    






    vec3 emittance = vec3(0.5f);

    vec3 tangent, bitangent;
    CreateCoordinateSystem(worldNrm, tangent, bitangent);
    vec3 rayOrigin = worldPos;
    vec3 rayDirection = SamplingHemisphere(prd.seed, tangent, bitangent, worldNrm);

    const float cos_theta = dot(rayDirection, worldNrm);
    const float p = cos_theta / M_PI;

    vec3 albedo = vec3(0.5f);
    vec3 BRDF = albedo / M_PI;

     prd.rayOrigin    = rayOrigin;
     prd.rayDirection = rayDirection;
     prd.hitValue     = emittance;
     prd.weight       = BRDF * cos_theta / p;
     return;

     if(prd.depth < 10)
     {
        prd.depth++;
        float tMin  = 0.001;
        float tMax  = 100000000.0;
        uint  flags = gl_RayFlagsOpaqueEXT;
        traceRayEXT(topLevelAS,    // acceleration structure
                    flags,         // rayFlags
                    0xFF,          // cullMask
                    0,             // sbtRecordOffset
                    0,             // sbtRecordStride
                    0,             // missIndex
                    rayOrigin,     // ray origin
                    tMin,          // ray min range
                    rayDirection,  // ray direction
                    tMax,          // ray max range
                    0              // payload (location = 0)
        );
     }
     vec3 incoming = prd.hitValue;

     // Apply the Rendering Equation here.
     prd.hitValue = emittance + (BRDF * incoming * cos_theta / p);
}

/*****************************************************************************************/