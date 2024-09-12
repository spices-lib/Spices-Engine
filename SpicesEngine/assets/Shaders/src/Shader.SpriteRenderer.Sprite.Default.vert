/**
* @file Shader.SpriteRenderer.Sprite.Default.vert.
* @brief This Shader Defines Sprite Renderer Sprite SubPass Default Vertex Shader Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive : enable    /* @brief Enable include Macro. */

#include "Header/ShaderVertexInput.glsl"
#include "Header/ShaderPreRendererLayout.glsl"
#include "Header/ShaderMeshDescLayout.glsl"

/*****************************************************************************************/

/************************************Vertex Output****************************************/

/**
* @brief Vertex Shader Output to Fragment Shader.
*/
layout(location = 0) out struct FragInput 
{
    vec2 texCoord;
} 
vertOut;

/*****************************************************************************************/

/*********************************Push Constant*******************************************/

/**
* @brief push constant.
*/
layout(push_constant) uniform Push 
{
    uint64_t descAddress;
} 
push;

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
    ExplainMeshDesciption(push.descAddress);
    vec3 pos = mat3(view.inView) * position;

    vertOut.texCoord = position.xy + vec2(0.5f);
    gl_Position = view.projection * view.view  * model * vec4(pos, 1.0);
}

/*****************************************************************************************/