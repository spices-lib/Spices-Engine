/**
* @file Shader.RayTracingRenderer.RayTracing.DefaultShadow.rmiss.
* @brief This Shader Defines RayTracing Renderer RayTracing SubPass DefaultShadow Ray Missing Shader Behaver.
* @author Spiecs.
*/

/************************************Pre Compile******************************************/

#version 460

#extension GL_EXT_ray_tracing : require     /* @brief Enable Ray Tracing Shader.  */

/*****************************************************************************************/

/************************************Missing Input Output*********************************/

layout(location = 1) rayPayloadInEXT bool isShadowed;

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
    isShadowed = false;
}

/******************************************************************************************/