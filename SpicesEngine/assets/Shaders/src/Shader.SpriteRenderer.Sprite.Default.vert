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
    mat4 model  = Model(desc.modelAddredd).i[0];
    vec3 pos = mat3(view.inView) * position;

    vertOut.texCoord = texCoord;
    gl_Position = view.projection * view.view  * model * vec4(pos, 1.0);
}

/*****************************************************************************************/