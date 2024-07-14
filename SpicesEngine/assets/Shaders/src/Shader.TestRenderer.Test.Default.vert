/**
* @file Shader.TestRenderer.Test.Default.vert.
* @brief This Shader Defines Test Renderer Test SubPass Default Vertex Shader Behaver.
* @author Spiecs.
*/

/************************************Pre Compile*******************************************/
		
#version 460

#extension GL_GOOGLE_include_directive : enable

#include "Header/ShaderVertexInput.glsl"

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main() 
{
	gl_Position = vec4(position * 2.0f, 1.0f);
}

/*****************************************************************************************/