/************************************Pre Compile*******************************************/

#version 460

/*****************************************************************************************/

/************************************Fragment Input***************************************/

/**
* @brief Fragment Shader Input From Geometry Shader.
*/
layout(location = 0) in struct FragInput {
    vec3 color;                            /*Fragmet Color*/
} fragInput;

/*****************************************************************************************/

/************************************Fragment Output**************************************/

/**
* @brief Fragment Shader Output to FrameBuffer.
*/
layout(location = 0) out vec4 outColor;                /*diffuse Attachment*/
layout(location = 1) out vec4 outNormal;               /*normal Attachment*/
layout(location = 2) out vec4 outSpecular;             /*specular Attachment*/
layout(location = 3) out vec4 outPosition;             /*position Attachment*/
layout(location = 4) out float outID;                  /*ID Attachment*/

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
    outColor = vec4(fragInput.color, 1.0f);
}

/*****************************************************************************************/