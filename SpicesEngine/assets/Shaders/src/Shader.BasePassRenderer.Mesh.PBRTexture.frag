/**
* @file Shader.BasePassRenderer.Mesh.PBRTexture.frag.
* @brief This Shader Defines BasePass Renderer Mesh SubPass PBRTexture Fragment Shader Behaver.
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
    int  albedoTexture;
    int  normalTexture;
    int  roughnessTexture;
    int  metallicTexture;
    int  maxRayDepth;
    int  maxLightDepth;
    int  maxShadowDepth;
};

/**
* @brief BasePassMesh Fragment Shader Entry Point.
*/
#include "Header/ShaderBasePassMeshPBR.glsl"

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void GetMaterialAttributes(const in Pixel pi, inout MaterialAttributes attributes)
{
    attributes.albedo           = texture(BindLessTextureBuffer[materialParam.albedoTexture], pixel.texCoord).xyz;
    attributes.roughness        = texture(BindLessTextureBuffer[materialParam.roughnessTexture], pixel.texCoord).x;
   //attributes.metallic        = texture(BindLessTextureBuffer[materialParam.metallicTexture],  pi.texCoord).x;
    attributes.emissive        = vec3(0.0f);
}

/*****************************************************************************************/