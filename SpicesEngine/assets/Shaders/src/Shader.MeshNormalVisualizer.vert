/**
* @file Shader.MeshNormalVisualizer.vert.
* @brief This Shader Defines MeshNormalVisualizer SubPass Default Vertex Shader Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive : enable    /* @brief Enable include Macro. */

#include "Header/ShaderVertexInput.glsl"
#include "Header/ShaderPreRendererLayout.glsl"

/*****************************************************************************************/

/************************************Vertex Output****************************************/

/**
* @brief Vertex Shader Output to Geometry Shader.
*/
layout(location = 0) out struct VertOut 
{
    vec3 position;                                 /* @brief World Position */
    vec3 normal;                                   /* @brief World Normal   */
} 
vertOut;

/*****************************************************************************************/

/*********************************Push Constant*******************************************/

/**
* @brief push constant.
*/
layout(push_constant) uniform Push 
{
    MeshDesc desc;                           /* @see MeshDesc. */
};

/*****************************************************************************************/

/************************************Specific Data****************************************/

layout(buffer_reference, scalar, buffer_reference_align = 8) buffer Model
{
    mat4 i[];
};

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
    mat4 model          = Model(desc.modelAddress).i[0];
    mat3 m3model        = mat3(transpose(inverse(model)));
    vec3 normal         = normalize(m3model * normal);

    vec4 pos = model * vec4(position, 1.0f);

    vertOut.position = pos.xyz;
    vertOut.normal = normal;

    gl_Position = pos;
}

/*****************************************************************************************/