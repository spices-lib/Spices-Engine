/**
* @file ShaderStructures.h.
* @brief This Shader Header File Defines Shader Structure.
* @author Spiecs.
*/

/************************************Pre Compile*******************************************/

#ifdef __cplusplus
#include <glm/glm.hpp>

using vec2 = glm::vec2;
using vec3 = glm::vec3;
using vec4 = glm::vec4;
using mat4 = glm::mat4;
using uint = unsigned int;

#endif

/*****************************************************************************************/

// Information of a obj model when referenced in a shader
/*struct ObjDesc
{
	int      txtOffset;             // Texture index offset in the array of textures
	uint64_t vertexAddress;         // Address of the Vertex buffer
	uint64_t indexAddress;          // Address of the index buffer
	uint64_t materialAddress;       // Address of the material buffer
	uint64_t materialIndexAddress;  // Address of the triangle material index buffer
};*/



// Push constant structure for the raster
struct PushConstantRaster
{
	mat4  modelMatrix;  // matrix of the instance
	vec3  lightPosition;
	uint  objIndex;
	float lightIntensity;
	int   lightType;
};


// Push constant structure for the ray tracer
struct PushConstantRay
{
	vec4  clearColor;
	vec3  lightPosition;
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