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
    vec3 cameraR = vec3(view.view[0][0], view.view[1][0], view.view[2][0]);
    vec3 cameraU = vec3(view.view[0][1], view.view[1][1], view.view[2][1]);
    vec3 cameraF = vec3(view.view[0][2], view.view[1][2], view.view[2][2]);

    vec3 pos = position.x * cameraR + position.y * cameraU + position.z * cameraF;

    vertOut.texCoord = texCoord;
    gl_Position = view.projection * view.view  * push.model * vec4(pos, 1.0);
}

/*****************************************************************************************/