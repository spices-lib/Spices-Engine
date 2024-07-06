/**
* @file ShaderLayouts.glsl.
* @brief This Shader Header File Defines Pre Shader DescriptorSets.
* @author Spiecs.
*/

/************************************Pre Compile*******************************************/

#include "ShaderCommon.h"

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

/*****************************************************************************************/