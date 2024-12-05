/**
* @file Shader.BasePassRenderer.Mesh.PBRGltf.rchit.
* @brief This Shader Defines RayTracing Renderer RayTracing SubPass PBRGltf Ray Closest Hit Shader Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive  : enable                    /* @brief Enable include Macro.           */

/**
* @brief Material Parameter.
* It should be the struct of all textures index and parameter buffer address.
* One index per texture, One address per buffer.
*/
struct MaterialParameter
{
    int   baseColorTexture;
    int   metallicRoughnessTexture;
    int   normalTexture;
    int   emissiveTexture;
    int   occlusionTexture;

    vec4  baseColorFactor;
    vec4  emissiveFactor;
    int   maxRayDepth;
    int   maxLightDepth;
    int   maxShadowDepth;
};

/**
* @brief Closest Hit Shader Entry Point.
*/
#include "Header/ShaderClosestHitPBR.glsl"

/*****************************************************************************************/

/******************************************Functions**************************************/

void GetMaterialAttributes(const in Pixel pi, inout MaterialAttributes attributes)
{
    if(materialParam.baseColorTexture > -0.5f)
    {
        attributes.albedo           = texture(BindLessTextureBuffer[materialParam.baseColorTexture], pi.texCoord).xyz * materialParam.baseColorFactor.xyz;
    }
    else
    {
        attributes.albedo           = materialParam.baseColorFactor.xyz;
    }
    
    if(materialParam.normalTexture > -0.5f)
    {
        attributes.normal           = pi.normal;
    }
    else
    {
        attributes.normal           = pi.normal;
    }
    
    if(materialParam.metallicRoughnessTexture > -0.5f)
    {
        float roughness             = texture(BindLessTextureBuffer[materialParam.metallicRoughnessTexture], pi.texCoord).g;
        float metallic              = texture(BindLessTextureBuffer[materialParam.metallicRoughnessTexture], pi.texCoord).b;
        
        attributes.roughness        = roughness;
        //attributes.metallic         = metallic;
    }
    else
    {
         attributes.roughness        = 0.5f;
        //attributes.metallic         = 0.5f;
    }
    
    if(materialParam.emissiveTexture > -0.5f)
    {
        attributes.emissive          = texture(BindLessTextureBuffer[materialParam.emissiveTexture], pi.texCoord).xyz * materialParam.emissiveFactor.xyz;
    }
    else
    {
        attributes.emissive          = materialParam.emissiveFactor.xyz;
    }

    attributes.maxRayDepth     = max(materialParam.maxRayDepth, 0);
    attributes.maxLightDepth   = max(materialParam.maxLightDepth, 0);
    attributes.maxShadowDepth  = max(materialParam.maxShadowDepth, 0);
}

/*****************************************************************************************/