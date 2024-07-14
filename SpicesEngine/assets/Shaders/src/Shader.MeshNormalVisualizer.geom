/**
* @file Shader.MeshNormalVisualizer.geom.
* @brief This Shader Defines MeshNormalVisualizer SubPass Default Geometry Shader Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive : enable     /* @brief Enable include Macro. */

#include "Header/ShaderPreRendererLayout.glsl"

/*****************************************************************************************/

/************************************Geometry Input***************************************/

/**
* @brief From Input Assembly.
*/
layout (triangles) in;

/**
* @brief Geometry Shader Input From Vertex Shader.
*/
layout(location = 0) in struct GeomInput {
    vec3 position;                         /* @brief World Postion */
    vec3 normal;                           /* @brief World Normal  */
} geomInput[];

/*****************************************************************************************/

/************************************Geometry Output**************************************/

/**
* @brief Switch Input Assembly from triangles to line_strip.
* Set Max 2 points per line.
*/
layout (line_strip, max_vertices = 2) out;

/**
* @brief Geometry Shader Output to Fragment Shader.
*/
layout(location = 0) out struct GeomOut {
    vec3 color;                            /* @brief Fragment Color */
} geomOut;

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main(void)
{
    for(int i = 0; i < gl_in.length(); i++)
    {
        vec3 pos = gl_in[i].gl_Position.xyz;
        vec3 normal = geomInput[i].normal;
        gl_Position = view.projection * view.view * vec4(pos, 1.0f);
        geomOut.color = vec3(1.0f, 1.0f, 0.0f);
        EmitVertex();

        gl_Position = view.projection * view.view * vec4(pos + normal * 0.2f, 1.0f);
        geomOut.color = vec3(1.0f, 1.0f, 0.0f);
        EmitVertex();

        EndPrimitive();
    }
}

/*****************************************************************************************/