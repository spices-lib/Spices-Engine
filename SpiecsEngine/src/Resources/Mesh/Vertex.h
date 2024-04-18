/**
* @file Vertex.h.
* @brief The Vertex Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
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
	class Vertex 
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		Vertex() {};

		/**
		* @brief Destructor Function.
		*/
		virtual ~Vertex() {};

		/**
		* @brief Vertex Position in local world.
		*/
		glm::vec3 position{};

		/**
		* @brief Vertex Normal.
		* @todo Normal World normal.
		*/
		glm::vec3 normal{};

		/**
		* @brief Vertex Color.
		*/
		glm::vec3 color{};

		/**
		* @brief Vertex UV.
		*/
		glm::vec2 texCoord{};

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