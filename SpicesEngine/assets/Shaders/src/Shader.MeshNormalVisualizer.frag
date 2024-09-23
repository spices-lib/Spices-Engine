/**
* @file Shader.MeshNormalVisualizer.frag.
* @brief This Shader Defines MeshNormalVisualizer SubPass Default Vertex Shader Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#version 460

/*****************************************************************************************/

/************************************Fragment Input***************************************/

/**
* @brief Fragment Shader Input From Geometry Shader.
*/
layout(location = 0) in struct FragInput 
{
    vec3 color;                                        /* @brief Fragmet Color */
} 
fragInput;

/*****************************************************************************************/

/************************************Fragment Output**************************************/

/**
* @brief Fragment Shader Output to FrameBuffer.
*/
layout(location = 0) out vec4  outAlbedo;                /* @brief albedo Attachment  */

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
    outAlbedo = vec4(fragInput.color, 1.0f);
}

/*****************************************************************************************/