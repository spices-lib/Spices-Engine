/**
* @file Shader.TestRenderer.Test.Default.geom.
* @brief This Shader Defines Test Renderer Test SubPass Default Geometry Shader Behaver.
* @author Spiecs.
*/

/************************************Pre Compile*******************************************/
		
#version 460

#define CUBE_FACE 6

/*****************************************************************************************/

/************************************Geometry Input***************************************/

layout(triangles, invocations = CUBE_FACE) in;

/*****************************************************************************************/

/************************************Geometry Output**************************************/

layout(triangle_strip, max_vertices = 3) out;
layout(location = 0) out vec3 color;

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
	for (int i = 0; i < gl_in.length(); i++)
	{
		gl_Layer = gl_InvocationID;
		vec4 pos = gl_in[i].gl_Position;
		gl_Position = pos;
		color = gl_Layer < 1 ? vec3(1.0f, 0.0f, 0.0f) : vec3(0.0f, 1.0f, 0.0f);
		EmitVertex();
	}

	EndPrimitive();
}

/*****************************************************************************************/