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
    uvec4 i[];           
};
Vertices vertices;

/**
* @brief Mesh PrimitivePoints Buffer.
*/
layout(buffer_reference, scalar, buffer_reference_align = 8) buffer PrimitivePoints
{
    uvec3 i[];
};
PrimitivePoints primitivePoints;

/**
* @brief Mesh PrimitiveVertices Buffer.
*/
layout(buffer_reference, scalar, buffer_reference_align = 8) buffer PrimitiveVertices
{
    uvec3 i[];
};
PrimitiveVertices primitiveVertices;

/**
* @brief Mesh PrimitiveLocations Buffer.
*/
layout(buffer_reference, scalar, buffer_reference_align = 8) buffer PrimitiveLocations
{
    uvec3 i[];
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
    primitiveVertices   = PrimitiveVertices(desc.primitiveVerticesAddress);
    primitiveLocations  = PrimitiveLocations(desc.primitiveLocationsAddress);
    meshlets            = Meshlets(desc.meshletsAddress);
    
#ifdef SHADER_BINDLESS_MATERIAL

    ExplainMaterialParameter(desc.materialParameterAddress);
    
#endif

}

/**
* @brief Get Vertices from PrimitiveVerteces.
* @param[in] primitiveID Primitive index.
* @return Returns Primitive Vertices.
*/
Vertex[3] GetVerticesUsingPrimitive(in uint primitiveID)
{
    uvec3 primVertex   = primitiveVertices.i[primitiveID];
                       
    uvec4 vti0         = vertices.i[primVertex.x];
    uvec4 vti1         = vertices.i[primVertex.y];
    uvec4 vti2         = vertices.i[primVertex.z];
                       
    Vertex v0;         
    v0.position        = positions.i[vti0.x];
    v0.normal          =   normals.i[vti0.y];
    v0.color           =    colors.i[vti0.z];
    v0.texCoord        = texCoords.i[vti0.w];
                       
    Vertex v1;         
    v1.position        = positions.i[vti1.x];
    v1.normal          =   normals.i[vti1.y];
    v1.color           =    colors.i[vti1.z];
    v1.texCoord        = texCoords.i[vti1.w];
                       
    Vertex v2;         
    v2.position        = positions.i[vti2.x];
    v2.normal          =   normals.i[vti2.y];
    v2.color           =    colors.i[vti2.z];
    v2.texCoord        = texCoords.i[vti2.w];
                       
    Vertex[3] vts      = { v0, v1, v2 };

    return vts;
}

/**
* @brief Get Pixel from Primitive Barycentric.
* @param[in] primitiveID Primitive index.
* @param[in] baryCoord Barycentric Coord.
* @return Returns Barycentric Pxiel.
*/
Pixel GetPixelUsingPrimitiveBarycentric(in uint primitiveID, in vec3 baryCoord)
{
    Vertex[3] vertices = GetVerticesUsingPrimitive(primitiveID);

    Pixel pixel;
    vec3 p             = vertices[0].position * baryCoord.x + vertices[1].position * baryCoord.y + vertices[2].position * baryCoord.z;
    vec3 n             = vertices[0].normal   * baryCoord.x + vertices[1].normal   * baryCoord.y + vertices[2].normal   * baryCoord.z;
    pixel.color        = vertices[0].color    * baryCoord.x + vertices[1].color    * baryCoord.y + vertices[2].color    * baryCoord.z;
    pixel.texCoord     = vertices[0].texCoord * baryCoord.x + vertices[1].texCoord * baryCoord.y + vertices[2].texCoord * baryCoord.z;

    pixel.position     = vec3(model * vec4(p, 1.0f));

    mat3 m3model       = mat3(transpose(inverse(model)));
    pixel.normal       = normalize(m3model * n);

    return pixel;
}

/*****************************************************************************************/

#endif