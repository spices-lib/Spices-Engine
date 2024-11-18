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
    uint  baseColorTexture;
    uint  metallicRoughnessTexture;
    uint  normalTexture;

    vec4  baseColorFactor;
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

void GetMaterialAttributes(in Pixel pi, inout MaterialAttributes attributes)
{
    attributes.albedo          = texture(BindLessTextureBuffer[materialParam.baseColorTexture], pi.texCoord).xyz * materialParam.baseColorFactor.xyz;
    attributes.roughness       = texture(BindLessTextureBuffer[materialParam.metallicRoughnessTexture], pi.texCoord).g;
    //attributes.metallic        = texture(BindLessTextureBuffer[materialParam.metallicRoughnessTexture],  pi.texCoord).b;
    attributes.emissive        = vec3(0.0f);
    attributes.maxRayDepth     = max(materialParam.maxRayDepth, 0);
    attributes.maxLightDepth   = max(materialParam.maxLightDepth, 0);
    attributes.maxShadowDepth  = max(materialParam.maxShadowDepth, 0);
}

/*****************************************************************************************/