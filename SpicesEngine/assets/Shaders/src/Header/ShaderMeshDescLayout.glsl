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
void ExplainMeshDesciption(const in uint64_t meshDescAddress)
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
* @brief Transform a Vertex to World.
* @param[in] vertex Vertex.
* @param[in] model model matrix.
* @return Returns Transformed Vertex.
*/
Vertex TransformVertexToWorld(const in Vertex vertex, const in mat4 model)
{
    mat3 m3model       = mat3(transpose(inverse(model)));
    
    Vertex trVertex;
    
    trVertex.position = vec3(model * vec4(vertex.position, 1.0f));
    trVertex.normal   = normalize(m3model * vertex.normal);
    trVertex.color    = vertex.color;
    trVertex.texCoord = vertex.texCoord;

    return trVertex;
}

/**
* @brief Get Vertices from PrimitiveVerteces.
* @param[in] primitiveID Primitive index.
* @return Returns Primitive Vertices.
*/
Vertex[3] GetVerticesUsingPrimitive(const in uint primitiveID)
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
* @brief Sort PrimLocation from min to max.
* @param[in] primLocation Primitive Location.
* @return Returns PrimLocation Order.
* @attention Not in use right now.
*/
uvec3 GetPrimLocationOrder(const in uvec3 primLocation)
{
    uvec3 order;
    
    uint max = max(max(primLocation.x, primLocation.y), primLocation.z);
    uint min = min(min(primLocation.x, primLocation.y), primLocation.z);

    if(primLocation.x == max) order.z = 0;
    if(primLocation.y == max) order.z = 1;
    if(primLocation.z == max) order.z = 2;
    
    if(primLocation.x == min) order.x = 0;
    if(primLocation.y == min) order.x = 1;
    if(primLocation.z == min) order.x = 2;
    
    if(primLocation.x != max && primLocation.x != min) order.y = 0;
    if(primLocation.y != max && primLocation.y != min) order.y = 1;
    if(primLocation.z != max && primLocation.z != min) order.y = 2;
    
    //return order;
    return uvec3(0, 1, 2);
}

/**
* @brief Get Pixel from Primitive Barycentric.
* @param[in] primitiveID Primitive index.
* @param[in] baryCoord Barycentric Coord.
* @return Returns Barycentric Pxiel.
* @attention Not in use right now.
*/
Pixel GetPixelUsingPrimitiveBarycentric(const in uint primitiveID, const in vec3 baryCoord)
{
    Vertex[3] vertices = GetVerticesUsingPrimitive(primitiveID);

    uvec3 primLocation = primitiveLocations.i[primitiveID];
    uvec3 order        = GetPrimLocationOrder(primLocation);

    Pixel pixel;
    vec3 p             = vertices[order.x].position * baryCoord.x + vertices[order.y].position * baryCoord.y + vertices[order.z].position * baryCoord.z;
    vec3 n             = vertices[order.x].normal   * baryCoord.x + vertices[order.y].normal   * baryCoord.y + vertices[order.z].normal   * baryCoord.z;
    pixel.color        = vertices[order.x].color    * baryCoord.x + vertices[order.y].color    * baryCoord.y + vertices[order.z].color    * baryCoord.z;
    pixel.texCoord     = vertices[order.x].texCoord * baryCoord.x + vertices[order.y].texCoord * baryCoord.y + vertices[order.z].texCoord * baryCoord.z;

    pixel.position     = vec3(model * vec4(p, 1.0f));

    mat3 m3model       = mat3(transpose(inverse(model)));
    pixel.normal       = normalize(m3model * n);

    return pixel;
}

/*****************************************************************************************/

#endif