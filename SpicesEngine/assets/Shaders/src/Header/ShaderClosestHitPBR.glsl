/**
* @file ShaderClosestHit.glsl.
* @brief This Shader Defines a Closest Hit Shader Basic Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#ifndef SHADER_CLOSEST_HIT
#define SHADER_CLOSEST_HIT

#extension GL_EXT_ray_tracing           : require   /* @brief Enable Ray Tracing Shader. */

#include "ShaderCommon.h"
#include "ShaderFunctionLibrary.glsl"
#include "ShaderPreRendererLayout.glsl"
#include "ShaderBindLessMaterial.glsl"
#include "ShaderMeshDescLayout.glsl"

/*****************************************************************************************/

/**********************************Closest Hit Input**************************************/

/** 
* @brief Hit Triangle Center Weight. 
*/
hitAttributeEXT vec3 attribs;

/*****************************************************************************************/

/*******************************Closest Hit Input Output**********************************/

/**
* @brief Ray trace payloads.
*/
layout(location = 0) rayPayloadInEXT HitPayLoad prd;

/**
* @brief True if is shadow area, data from Miss Shader.
*/
layout(location = 1) rayPayloadEXT bool isShadowArea;

/*****************************************************************************************/

/********************************Specific Renderer Data***********************************/

/**
* @brief Acceleration Structure.
*/
layout(set = 2, binding = 0) uniform accelerationStructureEXT topLevelAS;

/**
* @brief MeshDescription Buffer of all Mesh in World.
*/
layout(set = 3, binding = 0, scalar) readonly buffer MeshDescBuffer 
{ 
    uint64_t i[];
}
meshDescBuffer;

/**
* @brief DirectionalLight Buffer in World.
*/
layout(set = 3, binding = 1, scalar) readonly buffer DLightBuffer   
{ 
    DirectionalLight i[];   /* @see DirectionalLight. */
} 
dLightBuffer;

/**
* @brief PointLight Buffer in World.
*/
layout(set = 3, binding = 2, scalar) readonly buffer PLightBuffer   
{ 
    PointLight i[];         /* @see PointLight. */
} 
pLightBuffer;

/*****************************************************************************************/

/******************************************Functions**************************************/

/**
* @brief Unpack Pixel from MeshDescBuffer.
* @param[in] weight attribs.
* @return Returns the Pixel ray intersected.
* @see Pixel.
*/
Pixel UnPackPixel(in vec3 weight);
        
/**
* @brief Init Material Attributes.
* @param[in] pi Intersected Pixel.
* @return Return specific MaterialAttributes.
*/
MaterialAttributes InitMaterialAttributes(in Pixel pi);
        
/**
* @brief MaterialAttributes Post Handle.
* @param[in, out] attr MaterialAttributes.
*/
void PostHandleWithMaterialAttributes(in out MaterialAttributes attr);

/**
* @brief Calculate Point Lights contribution for pixel emissive.
* @param[in] pi Intersected Pixel.
* @param[in] attr MaterialAttributes.
* @return Returns the contribution of Point Lights.
*/
vec3 CalculatePointLights(in Pixel pi, in MaterialAttributes attr);

/**
* @brief Calculate Directional Lights contribution for pixel emissive.
* @param[in] pi Intersected Pixel.
* @param[in] attr MaterialAttributes.
* @return Returns the contribution of Directional Lights.
*/
vec3 CalculateDirectionalLights(in Pixel pi , in MaterialAttributes attr);

/**
* @brief Get Material Attributes, must be implementated by specific rchit shader.
* @param[in] pi Intersected Pixel.
* @param[in,out] attributes specific MaterialAttributes.
*/
void GetMaterialAttributes(in Pixel pi, inout MaterialAttributes attributes);

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
    Pixel pi = UnPackPixel(attribs);                                                                   /* @brief Get interest Pixel data.           */
    MaterialAttributes materialAttributes = InitMaterialAttributes(pi);                                /* @brief Init material attributes.          */
    GetMaterialAttributes(pi, materialAttributes);                                                     /* @brief Get material specific attributes.  */
    PostHandleWithMaterialAttributes(materialAttributes);                                              /* @brief Post handle materialAttributes.    */

    /**
    * @brief Calculate Next Ray recursion attributes.
    */
    vec3 tangent, bitangent;
    CreateCoordinateSystem(materialAttributes.normal, tangent, bitangent);
    
    vec3 rayDirection = normalize(mix(
        reflect(prd.rayDirection, materialAttributes.normal), 
        SamplingHemisphere(prd.seed, tangent, bitangent, materialAttributes.normal), 
        materialAttributes.roughness
    ));

    /**
    * @brief calaculate BRDF with Lights.
    * Skip if reach material maxLightDepth.
    */
    vec3 brdf_diffuse = BRDF_Diffuse_Lambert(materialAttributes.albedo);
    vec3 brdf_specular = vec3(0.0f);
    if(prd.rayDepth < materialAttributes.maxLightDepth)
    {
        brdf_specular += (CalculatePointLights(pi, materialAttributes) + CalculateDirectionalLights(pi, materialAttributes));
    }
    
    vec3 BRDF = brdf_diffuse + brdf_specular;
    
    const float cos_theta = dot(rayDirection, materialAttributes.normal);
    const float p = cos_theta / PI;

    /**
    * @brief Fill in rayPayloadInEXT.
    */
    prd.rayOrigin      = pi.position;
    prd.rayDirection   = rayDirection;
    prd.hitValue       = materialAttributes.emissive;
    prd.weight         = BRDF * cos_theta / p;
    prd.maxRayDepth    = materialAttributes.maxRayDepth;
    prd.entityID       = desc.entityID;
    prd.triangleID     = gl_PrimitiveID;
}

/*****************************************************************************************/

Pixel UnPackPixel(in vec3 weight)
{
    /**
    * @brief Access Buffer by GPU address.
    */
    ExplainMeshDesciption(meshDescBuffer.i[gl_InstanceCustomIndexEXT]);
    
    /**
    * @brief Get Vertices of the triangle.
    */ 
    Vertex[3] vts = UnpackVertexFromPrimitiveVertices(gl_PrimitiveID);
    
    const vec3 barycentrics = vec3(1.0 - weight.x - weight.y, weight.x, weight.y);

    /**
    * @brief Computing the coordinates of the hit position.
    */ 
    const vec3 localpos = vts[0].position * barycentrics.x + vts[1].position * barycentrics.y + vts[2].position * barycentrics.z;
    const vec3 worldPos = vec3(gl_ObjectToWorldEXT * vec4(localpos, 1.0));
    //const vec3 worldPos = gl_WorldRayOriginEXT + gl_WorldRayDirectionEXT * gl_HitTEXT; /* @brief another way, low accuracy. */
    
    /**
    * @brief Computing the normal at hit position.
    */ 
    const vec3 localnrm = vts[0].normal * barycentrics.x + vts[1].normal * barycentrics.y + vts[2].normal * barycentrics.z;
    const vec3 worldNrm = normalize(vec3(localnrm * gl_WorldToObjectEXT));
    
    /**
    * @brief Computing the color at hit position.
    */
    const vec3 color = vts[0].color * barycentrics.x + vts[1].color * barycentrics.y + vts[2].color * barycentrics.z;
    
    /**
    * @brief Computing the uv at hit position.
    */ 
    const vec2 uv = vts[0].texCoord * barycentrics.x + vts[1].texCoord * barycentrics.y + vts[2].texCoord * barycentrics.z;
    
    /**
    * @brief Make Pixel.
    */
    Pixel pi;
    pi.position   = worldPos;
    pi.normal     = worldNrm;
    pi.color      = color;
    pi.texCoord   = uv;
    
    return pi;
}

MaterialAttributes InitMaterialAttributes(in Pixel pi)
{
    MaterialAttributes attributes;
    
    attributes.albedo           = vec3(0.5f);     /* @brief 50% energy reflect.                   */
    attributes.roughness        = 1.0f;           /* @brief 100% random direction reflect.        */
    attributes.metallic         = 0.0f;
    attributes.emissive         = vec3(0.0f);     /* @brief self no energy.                       */
    attributes.normal           = pi.normal;      /* @brief Pixel World Normal.                   */
    attributes.maxRayDepth      = 1;              /* @brief Pixel Ray Tracing Max Depth.          */
    attributes.maxLightDepth    = 1;              /* @brief Pixel Ray Tracing Max Light Depth.    */
    attributes.maxShadowDepth   = 1;              /* @brief Pixel Ray Tracing Max Shadow Depth.   */
    
    return attributes;
}

void PostHandleWithMaterialAttributes(in out MaterialAttributes attr)
{
    attr.albedo    = clamp(attr.albedo, vec3(0.0f), vec3(1.0f));    /* @brief Clamp to  0.0f - 1.0f */
    attr.roughness = clamp(attr.roughness, 0.0f, 1.0f);             /* @brief Clamp to  0.0f - 1.0f */
    attr.metallic  = clamp(attr.metallic, 0.0f, 1.0f);              /* @brief Clamp to  0.0f - 1.0f */
    if(dot(-prd.rayDirection, attr.normal) < 0.0f)                  /* @brief Clamp to -1.0f - 1.0f */
    {
        attr.normal *= -1.0f;                                       /* @brief reverse normal in back face */
    }
    attr.normal         = normalize(attr.normal);
    attr.maxRayDepth    = max(0, attr.maxRayDepth);
    attr.maxLightDepth  = max(0, attr.maxLightDepth);
    attr.maxShadowDepth = max(0, attr.maxShadowDepth);
}

vec3 CalculatePointLights(in Pixel pi, in MaterialAttributes attr)
{
    vec3 col = vec3(0.0f);
 
    /**
    * @brief Iter all PointLights in Buffer.
    */
    for(int i = 0; i < pLightBuffer.i.length(); i++)
    {
        /**
        * @brief Get PointLight from Buffer.
        */
        PointLight light = pLightBuffer.i[i];

        /**
        * @brief If hit break condition, than break.
        */
        if(light.intensity < -500.0f) break;

        /**
        * @brief light position
        */ 
        vec3 lpos = light.position;
        vec3 dir  = normalize(lpos - pi.position);
        vec3 V    = normalize(prd.rayOrigin - pi.position);
            
        if(dot(attr.normal, dir) > 0)
        {
            float tMin   = 0.001f;
            float tMax   = length(lpos - pi.position);
            vec3  origin = pi.position;
            vec3  rayDir = dir;
            uint  flags  = gl_RayFlagsTerminateOnFirstHitEXT | gl_RayFlagsOpaqueEXT | gl_RayFlagsSkipClosestHitShaderEXT;
            isShadowArea = true;
            
            traceRayEXT(topLevelAS,          /* @brief Acceleration structure.  */
                        flags,               /* @brief RayFlags.                */
                        0xFF,                /* @brief CullMask.                */
                        0,                   /* @brief SBTRecordOffset.         */
                        0,                   /* @brief SBTRecordStride.         */
                        1,                   /* @brief MissIndex.               */
                        origin,              /* @brief Ray origin.              */
                        tMin,                /* @brief Ray min range.           */
                        rayDir,              /* @brief Ray direction.           */
                        tMax,                /* @brief Ray max range.           */
                        1                    /* @brief Payload (location = 1).  */
            );

            if(!isShadowArea)
            {
                float attenuation = 1.0f / (light.constantf + light.linear * tMax + light.quadratic * tMax * tMax);
                col += BRDF_Specular_CookTorrance(dir, V, attr.normal, light.color, attr.albedo, attr.metallic, attr.roughness) * light.intensity * attenuation;
            }
        }
    }
    
    return col;
}

vec3 CalculateDirectionalLights(in Pixel pi, in MaterialAttributes attr)
{
    vec3 col = vec3(0.0f);
    
    /**
    * @brief Iter all DirectionalLights in Buffer.
    */
    for(int i = 0; i < dLightBuffer.i.length(); i++)
    {
        /**
        * @brief Get PointLight from Buffer.
        */
        DirectionalLight light = dLightBuffer.i[i];
        
        /**
        * @brief If hit break condition, than break.
        */
        if(light.intensity < -500.0f) break;

        /**
        * @brief light position
        */ 
        vec4 dir4 = light.rotationMatrix * vec4(1.0f, 0.0f, 0.0f, 1.0f);
        vec3 dir  = dir4.xyz;
        vec3 V    = normalize(prd.rayOrigin - pi.position);

        if(dot(attr.normal, dir) > 0)
        {
            float tMin   = 0.001f;
            float tMax   = 100000.0f;
            vec3  origin = pi.position;
            vec3  rayDir = dir;
            uint  flags  = gl_RayFlagsTerminateOnFirstHitEXT | gl_RayFlagsOpaqueEXT | gl_RayFlagsSkipClosestHitShaderEXT;
            isShadowArea = true;
            
            traceRayEXT(topLevelAS,          /* @brief Acceleration structure.  */
                        flags,               /* @brief RayFlags.                */
                        0xFF,                /* @brief CullMask.                */
                        0,                   /* @brief SBTRecordOffset.         */
                        0,                   /* @brief SBTRecordStride.         */
                        1,                   /* @brief MissIndex.               */
                        origin,              /* @brief Ray origin.              */
                        tMin,                /* @brief Ray min range.           */
                        rayDir,              /* @brief Ray direction.           */
                        tMax,                /* @brief Ray max range.           */
                        1                    /* @brief Payload (location = 1).  */
            );

            if(!isShadowArea)
            {
                col += BRDF_Specular_CookTorrance(dir, V, attr.normal, light.color, attr.albedo, attr.metallic, attr.roughness) * light.intensity;
            }
        }
    }
    
    return col;
}

#endif