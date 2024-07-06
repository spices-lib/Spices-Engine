/**
* @file Vertex.h.
* @brief The Vertex Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "..\..\..\assets\Shaders\src\Header\ShaderCommon.h"
#include <vector>
#include <iostream>

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include <glm/gtx/hash.hpp>

namespace Spiecs {

	/**
	* @brief Vertex Class.
	* This class defines what Vertex input data.
	*/
	struct Vertex : public SpiecsShader::Vertex
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		Vertex() {};

		/**
		* @brief Destructor Function.
		* @attemtion Why Destructor causes bug here
		* @todo fix it.
		*/
		//virtual ~Vertex() = default;

		/**
		* @brief Get Vertex Input Binding Description.
		* @return Returns the Vertex Input Binding Description.
		*/
		static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();

		/**
		* @brief Get Vertex Input Attribute Description (VAO).
		* @return Returns the Vertex Input Attribute Description.
		*/
		static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();

		/**
		* @brief Assignment Operation.
		* @param[in] other Another Vertex.
		* @return Returns true if equal.
		*/
		bool operator==(const Vertex& other) const {
			return position == other.position && color == other.color && normal == other.normal && texCoord == other.texCoord;
		};
	};
}

namespace std {

	/**
	* @brief Hash function used for unordered_map.
	*/
	template<> struct hash<Spiecs::Vertex> {
		size_t operator()(Spiecs::Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.position) ^
					(hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^
					(hash<glm::vec3>()(vertex.color) << 1) >> 1 ^
					(hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}