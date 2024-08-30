/**
* @file Shader.SceneComposeRenderer.SceneCompose.Default.vert.
* @brief This Shader Defines SceneCompose Renderer SceneCompose SubPass Default Vertex Shader Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#version 460

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

/**********************************Shader Entry*******************************************/

void main()
{
	vertOut.texCoord = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
	gl_Position = vec4(vertOut.texCoord * 2.0f - 1.0f, 0.0f, 1.0f);
}

/*****************************************************************************************/