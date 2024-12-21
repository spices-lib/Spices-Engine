/**
* @file Shader.SlateRenderer.Slate.Default.vert.
* @brief This Shader Defines Slate Renderer Slate SubPass Default Vertex Shader Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#version 460

/*****************************************************************************************/

/*************************************Vertex Input****************************************/

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 color;

/*****************************************************************************************/

/************************************Vertex Output****************************************/

/**
* @brief Vertex Shader Output to Rasterization.
*/
out gl_PerVertex 
{
    vec4 gl_Position;
};

/**
* @brief Vertex Shader Output to Fragment Shader.
*/
layout(location = 0) out struct FragInput
{
    vec4 color;
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
    vec2 scale;
    vec2 translate;
} 
push;

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
    vertOut.color    = color;
    vertOut.texCoord = texCoord;

    gl_Position = vec4(position * push.scale + push.translate, 0, 1);
}

/*****************************************************************************************/