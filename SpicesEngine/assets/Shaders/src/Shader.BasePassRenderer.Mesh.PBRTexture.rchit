/**
* @file Shader.BasePassRenderer.Mesh.PBRTexture.rchit.
* @brief This Shader Defines RayTracing Renderer RayTracing SubPass PBRTexture Ray Closest Hit Shader Behaver.
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
    uint albedoTexture;
    uint normalTexture;
    uint roughnessTexture;
    uint metallicTexture;
    int  maxRayDepth;
    int  maxLightDepth;
    int  maxShadowDepth;
} 
materialParam;

/**
* @brief Closest Hit Shader Entry Point.
*/
#include "Header/ShaderClosestHitPBR.glsl"

/*****************************************************************************************/

/******************************************Functions**************************************/

void GetMaterialAttributes(in Pixel pi, inout MaterialAttributes attributes)
{
    attributes.albedo          = texture(BindLessTextureBuffer[materialParam.albedoTexture],    pi.texCoord).xyz;
    attributes.roughness       = texture(BindLessTextureBuffer[materialParam.roughnessTexture], pi.texCoord).x;
    //attributes.metallic        = texture(BindLessTextureBuffer[materialParam.metallicTexture],  pi.texCoord).x;
    attributes.emissive        = vec3(0.0f);
    attributes.maxRayDepth     = max(materialParam.maxRayDepth, 0);
    attributes.maxLightDepth   = max(materialParam.maxLightDepth, 0);
    attributes.maxShadowDepth  = max(materialParam.maxShadowDepth, 0);
}

/*****************************************************************************************/