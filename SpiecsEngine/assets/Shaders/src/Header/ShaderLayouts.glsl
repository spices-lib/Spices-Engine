/**
* @file ShaderLayouts.glsl.
* @brief This Shader Header File Defines Pre Shader DescriptorSets.
* @author Spiecs.
*/

/************************************Pre Compile*******************************************/

#include "ShaderStructures.h"

/*************************************Pre Renderer Data***********************************/

/**
* @brief Global View Struct.
*/
layout(set = 0, binding = 0) uniform View {
    mat4 projection;          /* @brief Projection Matrix from major Camera Entity   */
    mat4 view;                /* @brief View Matrix from major Camera Entity         */
    mat4 inView;              /* @brief Inverse View Matrix from major Camera Entity */
    vec4 sceneTextureSize;    /* @brief Scene Texture Size                           */
    vec4 windowSize;          /* @brief Application Window Size                      */
} view;

/**
* @brief Application Inout Struct.
*/
layout(set = 0, binding = 1) uniform SpiecsInput {
    vec4 mousePos;            /* @brief Mouse Postion and inverse position */
    float gameTime;           /* @brief Application Run time since start   */
    float frameTime;          /* @brief Duration time since last frame     */
} spiecsInput;

/*****************************************************************************************/