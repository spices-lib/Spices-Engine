/**
* @file ShaderVertexInput.glsl.
* @brief This Shader Header File Defines Vertex Input Structure.
* @author Spices.
*/

/************************************Pre Compile******************************************/

#ifndef SHADER_BINDLESS_MATERIAL
#define SHADER_BINDLESS_MATERIAL

#include "ShaderCommon.h"

/*****************************************************************************************/

/************************************Vertex Input*****************************************/

/**
* @brief Buffer of all Material Parameters in World.
* Only Access index 0, unless you want access other material parameter.
*/
layout(buffer_reference, scalar, buffer_reference_align = 8) buffer MaterialParameters { 
    MaterialParameter i[]; 
};

/*****************************************************************************************/

/******************************************Functions**************************************/

/**
* @brief Explain Material Parameter to split struct and texture.
* @param[in] materialParameterAddress the MaterialParameter address.
*/
void ExplainMaterialParameter(in uint64_t materialParameterAddress)
{
    materialParam      = MaterialParameters(materialParameterAddress).i[0];
}

/*****************************************************************************************/

#endif