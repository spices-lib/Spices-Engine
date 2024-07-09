/**
* @file ShaderVertexInput.glsl.
* @brief This Shader Header File Defines Vertex Input Structure.
* @author Spiecs.
*/

/************************************Pre Compile******************************************/

#ifndef SHADER_VERTEX_INPUT
#define SHADER_VERTEX_INPUT

/*****************************************************************************************/

/************************************Vertex Input*****************************************/

/**
* @brief Vertex Shader Input from InputAssembly.
*/
layout(location = 0) in vec3 position;             /* @brief Vertex Position. */
layout(location = 1) in vec3 normal;               /* @brief Vertex Normal.   */
layout(location = 2) in vec3 color;                /* @brief Vertex Color.    */
layout(location = 3) in vec2 texCoord;             /* @brief Vertex UV.       */

/*****************************************************************************************/

#endif