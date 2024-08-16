/**
* @file Shader.ShadowRenderer.DirectionalLightShadow.Default.geom.
* @brief This Shader Defines Shadow Renderer DirectionalLightShadow SubPass Default Geometry Shader Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#version 460

#define MAX_DIRECTIONALLIGHT_NUM 2

/*****************************************************************************************/

/************************************Geometry Input***************************************/

/**
* @brief From Input Assembly.
*/
layout(triangles, invocations = MAX_DIRECTIONALLIGHT_NUM) in;

/**
* @brief SSBO of DirectioaalLight Matrixs.
*/
layout(set = 2, binding = 0) readonly buffer DirectionalLightMatrixs 
{
	mat4 Matrixs[];
};

/*****************************************************************************************/

/************************************Geometry Output**************************************/

/**
* @brief Output to different layer in Framebuffer attachment Texture2DArray. 
*/
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