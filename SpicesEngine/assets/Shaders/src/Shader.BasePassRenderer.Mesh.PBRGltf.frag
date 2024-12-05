/**
* @file Shader.BasePassRenderer.Mesh.PBRGltf.frag.
* @brief This Shader Defines BasePass Renderer Mesh SubPass PBRGltf Fragment Shader Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive          : enable    /* @brief Enable include Macro.            */

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
* @brief BasePassMesh Fragment Shader Entry Point.
*/
#include "Header/ShaderBasePassMeshPBR.glsl"

/**********************************Shader Entry*******************************************/

void GetMaterialAttributes(const in Pixel pi, inout MaterialAttributes attributes)
{
    if(materialParam.baseColorTexture > -0.5f)
    {
        attributes.albedo           = texture(BindLessTextureBuffer[materialParam.baseColorTexture], pixel.texCoord).xyz * materialParam.baseColorFactor.xyz;
    }
    else
    {
        attributes.albedo           = materialParam.baseColorFactor.xyz;
    }
    
    if(materialParam.normalTexture > -0.5f)
    {
        attributes.normal           = pixel.normal;
    }
    else
    {
        attributes.normal           = pixel.normal;
    }
    
    if(materialParam.metallicRoughnessTexture > -0.5f)
    {
        float roughness     = texture(BindLessTextureBuffer[materialParam.metallicRoughnessTexture], pixel.texCoord).g;
        float metallic      = texture(BindLessTextureBuffer[materialParam.metallicRoughnessTexture], pixel.texCoord).b;
        
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
        attributes.emissive          += texture(BindLessTextureBuffer[materialParam.emissiveTexture], pixel.texCoord).xyz * materialParam.emissiveFactor.xyz;
    }
    else
    {
        attributes.emissive          += materialParam.emissiveFactor.xyz;
    }
}

/*****************************************************************************************/