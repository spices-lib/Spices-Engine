/**
* @file Shader.TestRenderer.Test.Default.vert.
* @brief This Shader Defines Test Renderer Test SubPass Default Vertex Shader Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/
		
#version 460

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main() 
{
	vec2 uv = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
	gl_Position = vec4(uv * 2.0f - 1.0f, 0.0f, 1.0f);
}

/*****************************************************************************************/