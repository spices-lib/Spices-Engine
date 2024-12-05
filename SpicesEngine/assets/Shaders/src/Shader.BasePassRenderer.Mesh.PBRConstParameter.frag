/**
* @file Shader.BasePassRenderer.Mesh.PBRConstParameter.frag.
* @brief This Shader Defines BasePass Renderer Mesh SubPass PBRConstParameter Fragment Shader Behaver.
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
    vec3  albedo;
    float roughness;
    float metallic;
    int   maxRayDepth;
    int   maxLightDepth;
    int   maxShadowDepth;
};

/**
* @brief BasePassMesh Fragment Shader Entry Point.
*/
#include "Header/ShaderBasePassMeshPBR.glsl"

/*****************************************************************************************/

/******************************************Functions**************************************/

void GetMaterialAttributes(const in Pixel pi, inout MaterialAttributes attributes)
{
    attributes.albedo          = materialParam.albedo;
    attributes.roughness       = materialParam.roughness;
    attributes.metallic        = materialParam.metallic;
    attributes.emissive        = vec3(0.0f);
}

/*****************************************************************************************/