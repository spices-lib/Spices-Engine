/**
* @file ShaderStructures.h.
* @brief This Shader Header File Defines Shader Structure.
* @author Spiecs.
*/

/************************************Pre Compile*******************************************/

#ifdef __cplusplus
#pragma once

#include <glm/glm.hpp>

namespace SpiecsShader
{
using vec2 = glm::vec2;     
using vec3 = glm::vec3;     
using vec4 = glm::vec4;     
using mat4 = glm::mat4;     
using uint = unsigned int;  

#else

#extension GL_EXT_scalar_block_layout                    : enable   /* @brief Enable shader vec3 type memory align. */
#extension GL_EXT_shader_explicit_arithmetic_types_int64 : require  /* @brief Enable uint64_t type in shader.       */

#endif

#define MESHBUFFERMAXNUM 100000
#define DIRECTIONALLIGHTBUFFERMAXNUM 100
#define POINTLIGHTBUFFERMAXNUM 10000

/*****************************************************************************************/

	
/***************************************Vertex********************************************/

/**
* @brief This Struct defines Per Vertex Struct of Mesh.
*/
struct Vertex
{
	vec3 position;            /* @brief Vertex Position in local world. */
	vec3 normal;              /* @brief Vertex Normal.                  */
	vec3 color;               /* @brief Vertex Color.                   */
	vec2 texCoord;            /* @brief Vertex UV.                      */
};

/*****************************************************************************************/


/*************************************Pre Renderer Data***********************************/

/**
* @brief Global View Struct.
*/
struct View 
{
	mat4 projection;          /* @brief Projection Matrix from major Camera Entity.                   */
	mat4 nprojection;         /* @brief Negative y axis Projection Matrix from major Camera Entity.   */
	mat4 view;                /* @brief View Matrix from major Camera Entity.                         */
	mat4 inView;              /* @brief Inverse View Matrix from major Camera Entity.                 */
	vec4 sceneTextureSize;    /* @brief Scene Texture Size.                                           */
	vec4 windowSize;          /* @brief Application Window Size.                                      */
};

/**
* @brief Application Inout Struct.
*/
struct Input 
{
	vec4  mousePos;            /* @brief Mouse Postion and inverse position. */
	float gameTime;            /* @brief Application Run time since start.   */
	float frameTime;           /* @brief Duration time since last frame.     */
};

/**
* @brief Push constant structure for the mesh basic
*/
struct PushConstantMesh 
{
	mat4 model;                /* @brief MeshPack ModelMatrix.             */
	int  entityID;             /* @brief EntityId, cast from entt::entity. */
};

/*****************************************************************************************/


/******************************************Light Data*************************************/

/**
* @brief This struct defines DirectionalLight data.
*/
struct DirectionalLight
{
	vec3  rotation;            /* @brief Rotation of SunLight.   */
	vec3  color;               /* @brief Color of SunLight.      */
	float intensity;           /* @brief Intensity of SunLight.  */
};

/**
* @brief This struct defines PointLight data.
*/
struct PointLight
{
	vec3  position;            /* @brief World Position of PointLight. */
	vec3  color;               /* @brief Color of PointLight.          */
	float intensity;           /* @brief Intensity of PointLight.      */
	float constantf;           /* @brief Constantf of PointLight.      */
	float linear;              /* @brief Linear of PointLight.         */
	float quadratic;           /* @brief Quadratic of PointLight.      */
};

/*****************************************************************************************/


/*********************************RayTracing Renderer Data********************************/

struct MeshDesc
{
	uint64_t vertexAddress;         /* Address of the Vertex buffer.                  */
	uint64_t indexAddress;          /* Address of the index buffer.                   */
};

struct HitPayLoad
{
	vec3  hitValue;
	float seed;
	int   depth;
	vec3  rayOrigin;
	vec3  rayDirection;
	vec3  weight;
};

// Push constant structure for the raster
struct PushConstantRaster
{
	mat4  modelMatrix;  // matrix of the instance
	vec3  lightPosition;
	uint  objIndex;
	float lightIntensity;
	int   lightType;
};

struct WaveFrontMaterial  // See ObjLoader, copy of MaterialObj, could be compressed for device
{
	vec3  ambient;
	vec3  diffuse;
	vec3  specular;
	vec3  transmittance;
	vec3  emission;
	float shininess;
	float ior;       // index of refraction
	float dissolve;  // 1 == opaque; 0 == fully transparent
	int   illum;     // illumination model (see http://www.fileformat.info/format/material/)
	int   textureId;
};


/******************************************Functions**************************************/



/*****************************************************************************************/

#ifdef __cplusplus
}
#endif