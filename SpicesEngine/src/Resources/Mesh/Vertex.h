/**
* @file Vertex.h.
* @brief The Vertex Class Definitions.
* @author Spices.
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

#include <src/meshoptimizer.h>

namespace Spices {

	/**
	* @brief Vertex Class.
	* This class defines what Vertex input data.
	*/
	struct Vertex : public SpicesShader::Vertex
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		Vertex() = default;

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

	/**
	* @brief Meshlet Class.
	* This class defines what Meshlet data.
	*/
	struct Meshlet : public SpicesShader::Meshlet
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		Meshlet() = default;

		/**
		* @brief Destructor Function.
		* @attemtion Why Destructor causes bug here
		* @todo fix it.
		*/
		//virtual ~Meshlet() = default;

		/**
		* @brief Copy data from MeshOpt Struct.
		* @param[in] m meshopt_Meshlet.
		* @param[in] bounds meshopt_Bounds.
		*/
		void FromMeshopt(const meshopt_Meshlet& m, const meshopt_Bounds& bounds);
	};

	/**
	* @brief Edge Class.
	* This class defines what Edge data.
	*/
	struct Edge : public SpicesShader::Edge
	{
	public:
		/**
		* @brief Constructor Function.
		*/
		Edge() = default;

		/**
		* @brief Destructor Function.
		* @attemtion Why Destructor causes bug here
		* @todo fix it.
		*/
		//virtual ~Edge() = default;

		/**
		* @brief Assignment Operation.
		* @param[in] other Another Vertex.
		* @return Returns true if equal.
		*/
		bool operator==(const Edge& other) const {
			return first == other.first && second == other.second;
		};
	};
}

namespace std {

	/**
	* @brief Hash function used for unordered_map.
	*/
	template<> struct hash<Spices::Vertex> {
		size_t operator()(Spices::Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.position) ^
					(hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^
					(hash<glm::vec3>()(vertex.color) << 1) >> 1 ^
					(hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};

	/**
	* @brief Hash function used for unordered_map.
	*/
	template<> struct hash<Spices::Edge> {
		size_t operator()(Spices::Edge const& edge) const {
			return ((hash<uint32_t>()(edge.first) ^
				(hash<uint32_t>()(edge.second) << 1)) >> 1);
		}
	};
}