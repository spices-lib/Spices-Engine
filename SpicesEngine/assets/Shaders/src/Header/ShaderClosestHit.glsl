/**
* @file ShaderClosestHit.glsl.
* @brief This Shader Defines a Closest Hit Shader Basic Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#ifndef SHADER_CLOSEST_HIT
#define SHADER_CLOSEST_HIT

#extension GL_EXT_ray_tracing           : require   /* @brief Enable Ray Tracing Shader.           */
#extension GL_EXT_nonuniform_qualifier  : enable    /* @brief Enable Bindless DescriptorSet.       */
#extension GL_EXT_buffer_reference2     : require   /* @brief Enable Shader Buffer Address access. */

#include "ShaderCommon.h"
#include "ShaderFunctionLibrary.glsl"

/**
* @brief Begin declear a const parameter struct by set binding.
* @example :
* 
* BEGIN_DECLEAR_CONST_PARAM(00)
* int   Param0;
* float Param1;
* vec2  Param2;
* END_DECLEAR_CONST_PARAM(00)
*
* EXPLAIN_CONST_PARAM(00)
*
* CONST_PARAM(00)
*/
#define BEGIN_DECLEAR_CONST_PARAM(setBinding)                  \
struct MaterialConstantParameter_##setBinding  {

/**
* @brief End declear a const parameter struct and buffer reference by set binding.
*/
#define END_DECLEAR_CONST_PARAM(setBinding)                    \
} constParam_##setBinding;                                     \
layout(buffer_reference, scalar, buffer_reference_align = 8)   \
buffer MaterialConstantParameters_##setBinding                 \
{ MaterialConstantParameter_##setBinding i[]; };

/**
* @brief Explain MaterialParameter to a const parameter struct instance.
*/
#define EXPLAIN_CONST_PARAM(setBinding)                        \
constParam_##setBinding = MaterialConstantParameters_##setBinding(param.address_##setBinding).i[0];

/**
* @brief Get const parameter struct instance by set binding.
*/
#define CONST_PARAM(setBinding)                                \
constParam_##setBinding

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
* @brief Buffer of all Material Parameters in World.
* Only Access index 0, unless you want access other material parameter.
*/
layout(buffer_reference, scalar, buffer_reference_align = 8) buffer MaterialParameters { 
    MaterialParameter i[]; 
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

/******************************************Functions**************************************/

/**
* @brief Unpack Vertex from MeshDescBuffer.
* @param[in] weight attribs.
* @return Returns the Vertex ray intersected.
* @see Vertex.
*/
Vertex UnPackVertex(in vec3 weight);

/**
* @brief Unpack Material Parameter from MaterialParameterBuffer.
* @return Returns the Material Parameter.
*/
MaterialParameter UnPackMaterialParameter();

/**
* @brief Explain Material Parameter to split struct and texture.
* @param[in] param the MaterialParameter needs to be explained.
*/
void ExplainMaterialParameter(in MaterialParameter param);

/**
* @brief Init Material Attributes.
* @param[in] vt Intersected Vertex.
* @return Return specific MaterialAttributes.
*/
MaterialAttributes InitMaterialAttributes(in Vertex vt);

/**
* @brief Calculate Point Lights contribution for pixel emissive.
* @param[in] vt Intersected Vertex.
* @param[in] attr MaterialAttributes.
* @return Returns the contribution of Point Lights.
*/
vec3 CalculatePointLights(in Vertex vt, in MaterialAttributes attr);

/**
* @brief Calculate Directional Lights contribution for pixel emissive.
* @param[in] vt Intersected Vertex.
* @param[in] attr MaterialAttributes.
* @return Returns the contribution of Directional Lights.
*/
vec3 CalculateDirectionalLights(in Vertex vt , in MaterialAttributes attr);

/**
* @brief Get Material Attributes, must be implementated by specific rchit shader.
* @param[in] vt Intersected Vertex.
* @param[in,out] attributes specific MaterialAttributes.
*/
void GetMaterialAttributes(in Vertex vt, inout MaterialAttributes attributes);

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
    /**
    * @brief Get interest Vertex data.
    */
    Vertex vt = UnPackVertex(attribs);
    
    /**
    * @brief Get material parameter data.
    */
    MaterialParameter parameter = UnPackMaterialParameter();
    
    /**
    * @brief Explain Material.
    */
    ExplainMaterialParameter(parameter);
    
    /**
    * @brief Init material attributes.
    */
    MaterialAttributes materialAttributes = InitMaterialAttributes(vt);
    
    /**
    * @brief Get material specific attributes.
    */
    GetMaterialAttributes(vt, materialAttributes);

    /**
    * @brief Reverse normal in back side.
    */
    if(dot(-prd.rayDirection, materialAttributes.normal) < 0.0f)
    {
        materialAttributes.normal *= -1.0f;
    }

    /**
    * @brief Add emissive with Lights.
    * Skip if reach material maxLightDepth.
    */
    if(prd.rayDepth < materialAttributes.maxLightDepth)
    {
        materialAttributes.emissive += CalculatePointLights(vt, materialAttributes) + CalculateDirectionalLights(vt, materialAttributes);
    }

    /**
    * @brief Calculate Next Ray recursion attributes.
    */
    vec3 tangent, bitangent;
    CreateCoordinateSystem(materialAttributes.normal, tangent, bitangent);
    
    vec3 rayOrigin    = vt.position;
    
    vec3 rayDirection = mix(
        reflect(prd.rayDirection, materialAttributes.normal), 
        SamplingHemisphere(prd.seed, tangent, bitangent, materialAttributes.normal), 
        clamp(materialAttributes.roughness, 0.0f, 1.0f)
    );
    
    const float cos_theta = dot(rayDirection, materialAttributes.normal);
    const float p = cos_theta / PI;

    vec3 BRDF = materialAttributes.albedo / PI;

    /**
    * @brief Fill in rayPayloadInEXT.
    */
    prd.rayOrigin      = rayOrigin;
    prd.rayDirection   = rayDirection;
    prd.hitValue       = materialAttributes.emissive;
    prd.weight         = BRDF * cos_theta / p;
    prd.maxRayDepth    = materialAttributes.maxRayDepth;
}

/*****************************************************************************************/

Vertex UnPackVertex(in vec3 weight)
{
    /**
    * @brief Access Buffer by GPU address.
    */
    MeshDesc desc       = meshDescBuffer.i[gl_InstanceCustomIndexEXT];
    Vertices vertices   = Vertices(desc.vertexAddress);
    Indices  indices    = Indices(desc.indexAddress);
    
    /**
    * @brief Get Indices of the triangle.
    */ 
    ivec3 index = indices.i[gl_PrimitiveID];
    
    /**
    * @brief Get Vertex of the triangle.
    */ 
    Vertex v0 = vertices.v[index.x];
    Vertex v1 = vertices.v[index.y];
    Vertex v2 = vertices.v[index.z];
    
    const vec3 barycentrics = vec3(1.0 - attribs.x - attribs.y, attribs.x, attribs.y);
    
    /**
    * @brief Computing the coordinates of the hit position.
    */ 
    const vec3 localpos = v0.position * barycentrics.x + v1.position * barycentrics.y + v2.position * barycentrics.z;
    const vec3 worldPos = vec3(gl_ObjectToWorldEXT * vec4(localpos, 1.0));
    //const vec3 worldPos = gl_WorldRayOriginEXT + gl_WorldRayDirectionEXT * gl_HitTEXT; /* @brief another way, low accuracy. */
    
    /**
    * @brief Computing the normal at hit position.
    */ 
    const vec3 localnrm = v0.normal * barycentrics.x + v1.normal * barycentrics.y + v2.normal * barycentrics.z;
    const vec3 worldNrm = normalize(vec3(localnrm * gl_WorldToObjectEXT));
    
    /**
    * @brief Computing the color at hit position.
    */
    const vec3 color = v0.color * barycentrics.x + v1.color * barycentrics.y + v2.color * barycentrics.z;
    
    /**
    * @brief Computing the uv at hit position.
    */ 
    const vec2 uv = v0.texCoord * barycentrics.x + v1.texCoord * barycentrics.y + v2.texCoord * barycentrics.z;
    
    /**
    * @brief Make Vertex.
    */
    Vertex vt;
    vt.position   = worldPos;
    vt.normal     = worldNrm;
    vt.color      = color;
    vt.texCoord   = uv;
    
    return vt;
}

MaterialParameter UnPackMaterialParameter()
{
    /**
    * @brief Access Buffer by GPU address.
    */
    MeshDesc desc       = meshDescBuffer.i[gl_InstanceCustomIndexEXT];
    
    /**
    * @brief If no vaild material parameter, just instance one and return.
    */
    if(desc.materialParameterAddress == 0)
    {
        MaterialParameter param;
        return param;
    }
    else
    {
        MaterialParameters params = MaterialParameters(desc.materialParameterAddress);
        return params.i[0];
    }
}

MaterialAttributes InitMaterialAttributes(in Vertex vt)
{
    MaterialAttributes attributes;
    
    attributes.albedo           = vec3(0.5f);     /* @brief 50% energy reflect.                   */
    attributes.roughness        = 1.0f;           /* @brief 100% random direction reflect.        */
    attributes.emissive         = vec3(0.0f);     /* @brief self no energy.                       */
    attributes.normal           = vt.normal;      /* @brief Pixel World Normal.                   */
    attributes.maxRayDepth      = 1;              /* @brief Pixel Ray Tracing Max Depth.          */
    attributes.maxLightDepth    = 1;              /* @brief Pixel Ray Tracing Max Light Depth.    */
    attributes.maxShadowDepth   = 1;              /* @brief Pixel Ray Tracing Max Shadow Depth.   */
    
    return attributes;
}

vec3 CalculatePointLights(in Vertex vt, in MaterialAttributes attr)
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
        vec3 dir = normalize(lpos - vt.position);

        if(dot(attr.normal, dir) > 0)
        {
            float tMin   = 0.001f;
            float tMax   = length(lpos - vt.position);
            vec3  origin = vt.position;
            vec3  rayDir = dir;
            uint  flags  = gl_RayFlagsTerminateOnFirstHitEXT | gl_RayFlagsOpaqueEXT | gl_RayFlagsSkipClosestHitShaderEXT;
            isShadowArea   = true;
            
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
                col += dot(attr.normal, dir) * light.color * light.intensity * attenuation;
            }
        }
    }
    
    return col;
}

vec3 CalculateDirectionalLights(in Vertex vt, in MaterialAttributes attr)
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
        vec3 dir = dir4.xyz;

        if(dot(attr.normal, dir) > 0)
        {
            float tMin   = 0.001f;
            float tMax   = 100000.0f;
            vec3  origin = vt.position;
            vec3  rayDir = dir;
            uint  flags  = gl_RayFlagsTerminateOnFirstHitEXT | gl_RayFlagsOpaqueEXT | gl_RayFlagsSkipClosestHitShaderEXT;
            isShadowArea   = true;
            
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
                col += dot(attr.normal, dir) * light.color * light.intensity;
            }
        }
    }
    
    return col;
}

#endif