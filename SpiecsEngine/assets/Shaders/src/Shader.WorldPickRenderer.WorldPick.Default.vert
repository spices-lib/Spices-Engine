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
layout(location = 0) out struct FragInput {
    vec2 texCoord;
} vertOut;

/*****************************************************************************************/

/*********************************Push Constant*******************************************/

/**
* @brief push constant.
*/
layout(push_constant) uniform Push {
    mat4 model;
    int entityID;
} push;

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
    vertOut.texCoord = texCoord;
    gl_Position = view.projection * view.view * push.model * vec4(position, 1.0);
}

/*****************************************************************************************/