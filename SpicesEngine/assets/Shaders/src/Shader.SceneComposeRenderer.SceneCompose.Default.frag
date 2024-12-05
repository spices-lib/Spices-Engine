/**
* @file Shader.SceneComposeRenderer.SceneCompose.Default.frag.
* @brief This Shader Defines SceneCompose Renderer SceneCompose SubPass Default Fragemt Shader Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive : enable     /* @brief Enable include Macro.       */
#extension GL_EXT_ray_tracing          : enable     /* @brief Enable Ray Tracing Shader.  */
#extension GL_EXT_ray_query            : enable     /* @brief Enable Ray Query Feature.   */

#include "Header/ShaderCommon.h"
#include "Header/ShaderPreRendererLayout.glsl"
#include "Header/ShaderFunctionLibrary.glsl"

/*****************************************************************************************/

/************************************Fragment Input***************************************/

/**
* @brief Fragment Shader Input From Vertex Shader.
*/
layout(location = 0) in struct FragInput 
{
	vec2 texCoord;                         /*Fragmet UV*/
} 
fragInput;

/*****************************************************************************************/

/************************************Fragment Output**************************************/

/**
* @brief Fragment Shader Output to FrameBuffer.
*/
layout(location = 0) out vec4 outSceneColor;    /*SceneColor Attachment*/

/*****************************************************************************************/

/********************************Specific Renderer Data***********************************/

/**
* @brief GBuffer struct like that:
* 0 - Albedo.
* 1 - Normal.
* 2 - Roughness.
* 3 - Metallic.
* 4 - Position.
*/

#define ALBEDO    0
#define NORMAL    1
#define ROUGHNESS 2
#define METALLIC  3
#define POSITION  4

/**
* @brief Subpass Input Attachments.
*/
layout(input_attachment_index = 0, set = 2, binding = 0) uniform subpassInput GBuffer[5];

/**
* @brief DirectionalLight Buffer in World.
*/
layout(set = 3, binding = 0, scalar) readonly buffer DLightBuffer 
{
	DirectionalLight i[];   /* @see DirectionalLight. */
} 
dLightBuffer;

/**
* @brief PointLight Buffer in World.
*/
layout(set = 3, binding = 1, scalar) readonly buffer PLightBuffer 
{
	PointLight i[];         /* @see PointLight. */
} 
pLightBuffer;

/**
* @brief Acceleration Structure.
*/
layout(set = 4, binding = 0) uniform accelerationStructureEXT topLevelAS;

/*****************************************************************************************/

/******************************************Functions**************************************/

/**
* @brief Get MaterialAttributes from GBuffer.
* @return Returns the Pixel MaterialAttributes.
* @see MaterialAttributes.
*/
GBufferPixel GetGBufferPixel();

/**
* @brief Calculate Point Lights contribution for pixel emissive.
* @param[in] gbp GBufferPixel.
* @return Returns the contribution of Point Lights.
*/
vec3 CalculatePointLights(in GBufferPixel gbp);

/**
* @brief Calculate Directional Lights contribution for pixel emissive.
* @param[in] gbp GBufferPixel.
* @return Returns the contribution of Directional Lights.
*/
vec3 CalculateDirectionalLights(in GBufferPixel gbp);

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
	GBufferPixel gbp = GetGBufferPixel();
	
	vec4 ro          = view.inView * vec4(0.0f, 0.0f, 0.0f, 1.0f);
    vec3 V           = normalize(ro.xyz - gbp.position);
    
    /**
    * @brief calaculate BRDF with Lights.
    * Different from raytracing.
    */
    vec3 brdf_diffuse = BRDF_Diffuse_Lambert(gbp.albedo) * PI;
    vec3 brdf_specular = vec3(0.0f);
    
    if(length(subpassLoad(GBuffer[NORMAL]).xyz) > 0.9f)
    {
        brdf_specular += (CalculatePointLights(gbp) + CalculateDirectionalLights(gbp));
    }
    vec3 BRDF = brdf_diffuse + brdf_specular;

	outSceneColor = vec4(BRDF, 1.0f);
}

/*****************************************************************************************/

GBufferPixel GetGBufferPixel()
{
	GBufferPixel gbp;
	
	gbp.albedo      = subpassLoad(GBuffer[ALBEDO]).xyz;
	gbp.normal      = normalize((subpassLoad(GBuffer[NORMAL]).xyz - vec3(0.5f)) * 2.0f);
	gbp.roughness   = subpassLoad(GBuffer[ROUGHNESS]).x;
	gbp.metallic    = subpassLoad(GBuffer[METALLIC]).x;
	gbp.position    = subpassLoad(GBuffer[POSITION]).xyz;
	
	return gbp;
}

vec3 CalculatePointLights(in GBufferPixel gbp)
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
        vec4 ro   = view.inView * vec4(0.0f, 0.0f, 0.0f, 1.0f);
        vec3 lpos = light.position;
        vec3 dir  = normalize(lpos - gbp.position);
        vec3 V    = normalize(ro.xyz - gbp.position);
        
        if(dot(gbp.normal, dir) > 0)
        {
            float tMin   = 0.001f;
            float tMax   = length(lpos - gbp.position);
            vec3  origin = gbp.position;
            vec3  rayDir = dir;
            uint  flags  = gl_RayFlagsTerminateOnFirstHitEXT;
            bool isShadowArea = false;

            rayQueryEXT rayQuery;

            rayQueryInitializeEXT(rayQuery,  /* @brief Ray result.              */
                topLevelAS,                  /* @brief Acceleration structure.  */
                flags,                       /* @brief RayFlags.                */
                0xFF,                        /* @brief CullMask.                */
                origin,                      /* @brief Ray origin.              */
                tMin,                        /* @brief Ray min range.           */
                rayDir,                      /* @brief Ray direction.           */
                tMax                         /* @brief Ray max range.           */
            );                               
          
            /**
            * @brief Traverse the acceleration structure and store information about the first intersection (if any).
            */
            rayQueryProceedEXT(rayQuery);

            /**
            * @brief If the intersection has hit a triangle, the fragment is shadowed.
            */
            if (rayQueryGetIntersectionTypeEXT(rayQuery, true) == gl_RayQueryCommittedIntersectionTriangleEXT) 
            {
                isShadowArea = true;
            }

            if(!isShadowArea)
            {
                float attenuation = 1.0f / (light.constantf + light.linear * tMax + light.quadratic * tMax * tMax);
                col += BRDF_Specular_CookTorrance(dir, V, gbp.normal, light.color, gbp.albedo, gbp.metallic, gbp.roughness) * light.intensity * attenuation;
            }
        }
    }
    
    return col;
}

vec3 CalculateDirectionalLights(in GBufferPixel gbp)
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
        vec4 ro   = view.inView * vec4(0.0f, 0.0f, 0.0f, 1.0f);
        vec4 dir4 = light.rotationMatrix * vec4(1.0f, 0.0f, 0.0f, 1.0f);
        vec3 dir  = dir4.xyz;
        vec3 V    = normalize(ro.xyz - gbp.position);

        if(dot(gbp.normal, dir) > 0)
        {
            float tMin   = 0.001f;
            float tMax   = 100000.0f;
            vec3  origin = gbp.position;
            vec3  rayDir = dir;
            uint  flags  = gl_RayFlagsTerminateOnFirstHitEXT;
            bool isShadowArea = false;
            
            rayQueryEXT rayQuery;

            rayQueryInitializeEXT(rayQuery,  /* @brief Ray result.              */
                topLevelAS,                  /* @brief Acceleration structure.  */
                flags,                       /* @brief RayFlags.                */
                0xFF,                        /* @brief CullMask.                */
                origin,                      /* @brief Ray origin.              */
                tMin,                        /* @brief Ray min range.           */
                rayDir,                      /* @brief Ray direction.           */
                tMax                         /* @brief Ray max range.           */
            );

            /**
            * @brief Traverse the acceleration structure and store information about the first intersection (if any).
            */
            rayQueryProceedEXT(rayQuery);

            /**
            * @brief If the intersection has hit a triangle, the fragment is shadowed.
            */
            if (rayQueryGetIntersectionTypeEXT(rayQuery, true) == gl_RayQueryCommittedIntersectionTriangleEXT)
            {
                isShadowArea = true;
            }

            if(!isShadowArea)
            {
                col += BRDF_Specular_CookTorrance(dir, V, gbp.normal, light.color, gbp.albedo, gbp.metallic, gbp.roughness) * light.intensity;
            }
        }
    }
    
    return col;
}