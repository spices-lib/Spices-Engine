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

/**
* @brief Ray trace payloads.
*/
layout(location = 1) rayPayloadInEXT bool isShadowArea;

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
    isShadowArea = false;
}

/******************************************************************************************/