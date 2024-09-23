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
layout(location = 0) out vec4  outColor;                /* @brief diffuse Attachment  */
layout(location = 1) out vec4  outNormal;               /* @brief normal Attachment   */
layout(location = 2) out vec4  outSpecular;             /* @brief specular Attachment */
layout(location = 3) out vec4  outPosition;             /* @brief position Attachment */
layout(location = 4) out float outEntityID;             /* @brief ID Attachment       */

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
    outColor = vec4(fragInput.color, 1.0f);
}

/*****************************************************************************************/