/**
* @file Shader.ShadowRenderer.DirectionalLightShadow.Default.geom.
* @brief This Shader Defines Shadow Renderer DirectionalLightShadow SubPass Default Geometry Shader Behaver.
* @author Spiecs.
*/

/************************************Pre Compile*******************************************/

#version 460

/*****************************************************************************************/

/************************************Geometry Input***************************************/

#define MAX_DIRECTIONALLIGHT_NUM 2

layout(triangles, invocations = MAX_DIRECTIONALLIGHT_NUM) in;

layout(set = 1, binding = 0) readonly buffer DirectionalLightMatrixs {
	mat4 Matrixs[];
};

/*****************************************************************************************/

/************************************Geometry Output**************************************/

layout(triangle_strip, max_vertices = 3) out;

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
	for (int i = 0; i < gl_in.length(); i++)
	{
		gl_Layer = gl_InvocationID;
		gl_Position = Matrixs[gl_InvocationID] * gl_in[i].gl_Position;
		EmitVertex();
	}

	EndPrimitive();
}

/*****************************************************************************************/