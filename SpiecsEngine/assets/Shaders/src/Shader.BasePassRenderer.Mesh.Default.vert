/**
* @file Shader.BasePassRenderer.Mesh.Default.vert.
* @brief This Shader Defines BasePass Renderer Mesh SubPass Default Vertex Shader Behaver.
* @author Spiecs.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive : enable

#include "Header/ShaderVertexInput.glsl"
#include "Header/ShaderLayouts.glsl"

/*****************************************************************************************/

/************************************Vertex Output****************************************/

/**
* @brief Vertex Shader Output to Fragment Shader.
*/
layout(location = 0) out struct VertOut {
    vec3 position;                                 /* @brief World Position */
    vec3 normal;                                   /* @brief World Position */
    vec3 color;                                    /* @brief Fragment Color */
    vec2 texCoord;                                 /* @brief Fragment UV    */
} vertOut;

/*****************************************************************************************/

/*********************************Push Constant*******************************************/

/**
* @brief push constant.
*/
layout(push_constant) uniform Push {
	PushConstantMesh push;
};

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
    mat3 m3model = mat3(transpose(inverse(push.model)));
    vec3 normal = normalize(m3model * normal);

    vec4 pos = push.model * vec4(position, 1.0f);

    vertOut.position = pos.xyz;
    vertOut.normal = normal;
    vertOut.color = color;
    vertOut.texCoord = texCoord;
    
    gl_Position = view.projection * view.view * pos;
}

/*****************************************************************************************/