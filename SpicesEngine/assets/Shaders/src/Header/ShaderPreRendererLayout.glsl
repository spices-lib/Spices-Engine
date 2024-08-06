/**
* @file ShaderPreRendererLayout.glsl.
* @brief This Shader Header File Defines Pre Shader DescriptorSets.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#ifndef SHADER_PRERENDERER_LAYOUT
#define SHADER_PRERENDERER_LAYOUT

#include "ShaderCommon.h"

/*****************************************************************************************/

/*************************************Pre Renderer Data***********************************/

/**
* @brief Global View Struct.
*/
layout(set = 0, binding = 0) uniform ShaderView {
    View view;                        /* @see View */
};

/**
* @brief Application Inout Struct.
*/
layout(set = 0, binding = 1) uniform ShaderInput {
    Input spiecsInput;          /* @see SpiecsInput */
};

/**
* @brief BindLess Texture Buffer.
*/
layout (set = BINDLESS_TEXTURE_SET, binding = BINDLESS_TEXTURE_BINDING) uniform sampler2D BindLessTextureBuffer[];

/*****************************************************************************************/

#endif