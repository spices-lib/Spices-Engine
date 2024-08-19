/**
* @file ShaderMeshDescLayout.glsl.
* @brief This Shader Header File Defines Mesh Descriptions.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#ifndef SHADER_MESHDESC_LAYOUT
#define SHADER_MESHDESC_LAYOUT

#include "ShaderCommon.h"

/*****************************************************************************************/

/****************************************Buffer Data**************************************/

/**
* @brief Mesh Desciption Buffer.
*/
layout(buffer_reference, scalar, buffer_reference_align = 8) buffer MeshDescs
{
    MeshDesc i[];             /* @see MeshDesc. */
};
MeshDesc desc;

/**
* @brief Mesh Model Matrix Buffer.
*/
layout(buffer_reference, scalar, buffer_reference_align = 8) buffer Models
{
    mat4 i[];
};
mat4 model;

/**
* @brief Mesh Vertices Buffer.
*/
layout(buffer_reference, scalar, buffer_reference_align = 8) buffer Vertices 
{ 
    Vertex v[];           
};
Vertices vertices;

/**
* @brief Mesh Indices Buffer.
*/
layout(buffer_reference, scalar, buffer_reference_align = 8) buffer Indices 
{ 
    ivec3 i[]; 
};
Indices indices;

/**
* @brief Mesh Meshlets Buffer.
*/
layout(buffer_reference, scalar, buffer_reference_align = 8) buffer Meshlets 
{
    Meshlet i[];
};
Meshlets meshlets;

/*****************************************************************************************/

/******************************************Functions**************************************/

/**
* @brief Explain Mesh Desciption to split variable.
* @param[in] meshDescAddress the Mesh Desciption Buffer Address address.
*/
void ExplainMeshDesciption(in uint64_t meshDescAddress)
{
    desc       = MeshDescs(meshDescAddress).i[0];
    model      = Models(desc.modelAddress).i[0];
    vertices   = Vertices(desc.vertexAddress);
    indices    = Indices(desc.indexAddress);
    meshlets   = Meshlets(desc.meshletAddress);
    
#ifdef SHADER_BINDLESS_MATERIAL

    ExplainMaterialParameter(desc.materialParameterAddress);
    
#endif

}

/*****************************************************************************************/

#endif