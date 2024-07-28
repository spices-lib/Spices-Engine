/**
* @file ShaderCommon.h.
* @brief This Shader Header File Defines Shader Structure both Shader side and C++ side.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#ifndef SHADER_COMMON
#define SHADER_COMMON

#ifdef __cplusplus
#include <glm/glm.hpp>

namespace SpicesShader
{
using vec2 = glm::vec2;     
using vec3 = glm::vec3;     
using vec4 = glm::vec4;     
using mat4 = glm::mat4;     
using uint = unsigned int; 

#else

#extension GL_EXT_scalar_block_layout                    : enable   /* @brief Enable shader vec3 type memory align.                         */
#extension GL_EXT_shader_explicit_arithmetic_types_int64 : require  /* @brief Enable uint64_t type in shader.                               */
#extension GL_EXT_buffer_reference2                      : require  /* @brief Enable Shader Buffer Address access.                          */
#extension GL_EXT_nonuniform_qualifier                   : enable   /* @brief Enable Bindless DescriptorSet.                                */

#ifdef SPICES_DEBUG

#extension GL_EXT_debug_printf                           : require  /* @brief Enable shader debug info. (debugPrintfEXT("Hello: %d", num);) */

#endif

#endif

#define MAXBINDLESSTEXTURECOUNT      65536
#define BINDLESSTEXTURESET           1
#define BINDLESSTEXTUREBINDING       0
#define MESHBUFFERMAXNUM             100000
#define DIRECTIONALLIGHTBUFFERMAXNUM 100
#define POINTLIGHTBUFFERMAXNUM       10000
#define PI                           3.14159265f
#define BIAS                         0.01f

#define invAtan                      vec2(0.1591, 0.3183)

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

/**
* @brief Using Pixel rather than Vertex in shader mey be easier to understand.
* It defines pixel data, not be original vertex data.
*/
#define Pixel Vertex

/**
* @brief This Struct defines PBR Material Attribute.
*/
struct MaterialAttributes
{
	vec3  albedo;              /* @brief Albedo, used for Reflection attenuation.                       */
	float roughness;           /* @brief Roughness, used for Reflection direction.                      */
	float metallic;            /* @brief Metallic, used for                                             */
	vec3  emissive;            /* @brief Emissive, used for Direct illumination.                        */
	vec3  normal;              /* @brief Normal, used for light effect.                                 */
	int   maxRayDepth;         /* @brief Max Ray Tracing Depth, defined specific from material.         */
	int   maxLightDepth;       /* @brief Max Ray Tracing Light Depth, defined specific from material.   */
	int   maxShadowDepth;      /* @brief Max Ray Tracing Shadow Depth, defined specific from material.  */
};

/**
 * @brief Gbuffer Data per pixel, for Fragment Shader use.
 */
struct GBufferPixel
{
	vec3  albedo;              /* @brief Albedo.     */
	vec3  normal;              /* @brief Normal.     */
	float roughness;           /* @brief Roughness.  */
	float metallic;            /* @brief Metallic.   */
	vec3  position;            /* @brief Position.   */
	float depth;               /* @brief Depth.      */
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
	uint stableFrames;        /* @brief Camera Stable Frames Number.                                  */
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

/*****************************************************************************************/


/******************************************Light Data*************************************/

/**
* @brief This struct defines DirectionalLight data.
*/
struct DirectionalLight
{
	mat4  rotationMatrix;      /* @brief Rotation of SunLight.   */
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
	uint64_t vertexAddress;                   /* Address of the Vertex buffer.                  */
	uint64_t indexAddress;                    /* Address of the index buffer.                   */
	uint64_t materialParameterAddress;        /* Address of the Material Parameter buffer.      */
	uint     verticesCount;                   /* Vertices Count.                                */
	uint     indicesCount;                    /* Indices Count.                                 */
	uint     entityID;                        /* @brief EntityId, cast from entt::entity.       */
};

/**
* @brief Ray Tracing Shader Data Location 0. 
*/
struct HitPayLoad
{
	vec3 hitValue;                  /* @brief Current Hit Fragment Color.                                        */
	uint seed;                      /* @brief Random Seed for BRDF Lambertian Light Reflection Direction Select. */
	int  rayDepth;                  /* @brief Ray Tracing Depth for diffuse.                                     */
	vec3 rayOrigin;                 /* @brief Next Ray Original World Position.                                  */
	vec3 rayDirection;              /* @brief Next Ray Direction in World Space.                                 */
	vec3 weight;                    /* @brief Current Fragment Color Weight.                                     */
	int  maxRayDepth;               /* @brief Max Ray Tracing Depth, defined specific from material.             */
	uint  entityID;                 /* @brief Entity ID from Closeest Hit to Ray Gen Shader to RayID Image.      */
};

/*****************************************************************************************/

/**
* @brief Push constant structure for the mesh basic
*/
struct PushConstantMesh
{
	mat4     model;                        /* @brief MeshPack ModelMatrix.              */
	MeshDesc desc;                         /* MeshDescription of MeshPack.              */
};

#ifdef __cplusplus
}
#endif

#endif