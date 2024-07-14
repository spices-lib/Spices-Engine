/**
* @file Shader.ShadowRenderer.DirectionalLightShadow.Default.frag.
* @brief This Shader Defines Shadow Renderer DirectionalLightShadow SubPass Default Fragment Shader Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#version 460

/*****************************************************************************************/

/************************************Fragment Output**************************************/

/**
* @brief Fragment Shader Output to FrameBuffer.
*/
layout(location = 0) out vec4 outColor;

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
    outColor = vec4(1.0f);
}

/*****************************************************************************************/