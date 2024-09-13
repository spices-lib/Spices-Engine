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
* @brief Positions Buffer.
*/
layout(buffer_reference, scalar, buffer_reference_align = 8) buffer Positions
{
    vec3 i[];
};
Positions positions;

/**
* @brief Normals Buffer.
*/
layout(buffer_reference, scalar, buffer_reference_align = 8) buffer Normals
{
    vec3 i[];
};
Normals normals;

/**
* @brief Colors Buffer.
*/
layout(buffer_reference, scalar, buffer_reference_align = 8) buffer Colors
{
    vec3 i[];
};
Colors colors;

/**
* @brief TexCoords Buffer.
*/
layout(buffer_reference, scalar, buffer_reference_align = 8) buffer TexCoords
{
    vec2 i[];
};
TexCoords texCoords;

/**
* @brief Mesh Vertices Buffer.
*/
layout(buffer_reference, scalar, buffer_reference_align = 8) buffer Vertices 
{ 
    ivec4 i[];           
};
Vertices vertices;

/**
* @brief Mesh PrimitivePoints Buffer.
*/
layout(buffer_reference, scalar, buffer_reference_align = 8) buffer PrimitivePoints
{
    ivec3 i[];
};
PrimitivePoints primitivePoints;

/**
* @brief Mesh PrimitiveLocations Buffer.
*/
layout(buffer_reference, scalar, buffer_reference_align = 8) buffer PrimitiveLocations
{
    ivec3 i[];
};
PrimitiveLocations primitiveLocations;

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
    desc                = MeshDescs(meshDescAddress).i[0];
    model               = Models(desc.modelAddress).i[0];
    positions           = Positions(desc.positionsAddress);
    normals             = Normals(desc.normalsAddress);
    colors              = Colors(desc.colorsAddress);
    texCoords           = TexCoords(desc.texCoordsAddress);
    vertices            = Vertices(desc.verticesAddress);
    primitivePoints     = PrimitivePoints(desc.primitivePointsAddress);
    primitiveLocations  = PrimitiveLocations(desc.primitiveLocationsAddress);
    meshlets            = Meshlets(desc.meshletsAddress);
    
#ifdef SHADER_BINDLESS_MATERIAL

    ExplainMaterialParameter(desc.materialParameterAddress);
    
#endif

}

/**
* @brief Unpack Vertices from PrimitiveVerteces.
* @param[in] primitiveID Primitive index.
* @return Returns Primitive Vertices.
*/
Vertex[3] UnpackVertexFromPrimitiveVertices(in uint primitiveID)
{
    ivec4 vti0 = vertices.i[3 * primitiveID + 0];
    ivec4 vti1 = vertices.i[3 * primitiveID + 1];
    ivec4 vti2 = vertices.i[3 * primitiveID + 2];

    Vertex v0;
    v0.position  = positions.i[vti0.x];
    v0.normal    =   normals.i[vti0.y];
    v0.color     =    colors.i[vti0.z];
    v0.texCoord  = texCoords.i[vti0.w];

    Vertex v1;
    v1.position  = positions.i[vti1.x];
    v1.normal    =   normals.i[vti1.y];
    v1.color     =    colors.i[vti1.z];
    v1.texCoord  = texCoords.i[vti1.w];

    Vertex v2;
    v2.position  = positions.i[vti2.x];
    v2.normal    =   normals.i[vti2.y];
    v2.color     =    colors.i[vti2.z];
    v2.texCoord  = texCoords.i[vti2.w];

    Vertex[3] vts = { v0, v1, v2 };

    return vts;
}

/*****************************************************************************************/

#endif