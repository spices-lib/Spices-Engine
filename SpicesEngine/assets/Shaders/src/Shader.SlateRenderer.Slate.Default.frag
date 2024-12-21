/**
* @file Shader.SlateRenderer.Slate.Default.frag.
* @brief This Shader Defines Slate Renderer Slate SubPass Default Fragment Shader Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#version 460

/*****************************************************************************************/

/************************************Fragment Input***************************************/

/**
* @brief Fragment Shader Input From Vertex Shader.
*/
layout(location = 0) in struct FragInput
{
    vec4 color;
    vec2 texCoord;
} 
fragInput;

/*****************************************************************************************/

/************************************Fragment Output**************************************/

/**
* @brief Fragment Shader Output to SwapchainImage.
*/
layout(location = 0) out vec4 SwapchainImage;

/*****************************************************************************************/

/************************************Specific Data****************************************/

layout(set = 0, binding = 0) uniform sampler2D bindTexture;

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
    SwapchainImage = fragInput.color * texture(bindTexture, fragInput.texCoord);
}

/*****************************************************************************************/