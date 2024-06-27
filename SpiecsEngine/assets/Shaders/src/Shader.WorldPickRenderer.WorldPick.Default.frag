/************************************Pre Compile*******************************************/

#version 460

/*****************************************************************************************/

/************************************Fragment Input***************************************/

/**
* @brief Fragment Shader Input From Geometry Shader.
*/
layout(location = 0) in struct FragInput {
    vec2 texCoord;
} fragInput;

/*****************************************************************************************/

/************************************Fragment Output**************************************/

/**
* @brief Fragment Shader Output to FrameBuffer.
*/
layout(location = 0) out float outPick;

/*****************************************************************************************/

/*********************************Push Constant*******************************************/

/**
* @brief push constant.
*/
layout(push_constant) uniform Push {
    mat4 model;
    int entityID;
} push;

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
    outPick = push.entityID;

    /*float alpha = texture(samplers, fragInput.texCoord).w;
    if (alpha < 0.01f) discard;
    outPick = push.entityID;*/
}

/*****************************************************************************************/