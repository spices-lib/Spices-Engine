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
#include <memory>

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include <glm/gtx/hash.hpp>

#include <src/meshoptimizer.h>

namespace Spices {

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
	struct Edge : public SpicesShader::HalfEdge
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		Edge() = default;

		/**
		* @brief Constructor Function.
		* @param[in] f first.
		* @param[in] s seconf.
		*/
		Edge(uint32_t f, uint32_t s)
		{
			first = f;
			second = s;
		};

		/**
		* @brief Destructor Function.
		* @attemtion Why Destructor causes bug here
		* @todo fix it.
		*/
		//virtual ~Edge() = default;

		/**
		* @brief Assignment Operation.
		* @param[in] other Another Edge.
		* @return Returns true if equal.
		*/
		bool operator==(const Edge& other) const {
			return (first == other.first && second == other.second) ||
				   (first == other.second && second == other.first);
		};

		/**
		* @brief Less Operation.
		* @param[in] other Another Edge.
		* @return Returns true if less than other.
		*/
		bool operator<(const Edge& other) const {
			return first < other.first;
		}
	};

	/**
	* @brief HalfEdge Class.
	* This class defines what HalfEdge data.
	*/
	struct HalfEdge : public SpicesShader::HalfEdge
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		HalfEdge() = default;

		/**
		* @brief Constructor Function.
		* @param[in] f first.
		* @param[in] s seconf.
		*/
		HalfEdge(uint32_t f, uint32_t s)
		{
			first = f;
			second = s;
		};

		/**
		* @brief Destructor Function.
		* @attemtion Why Destructor causes bug here
		* @todo fix it.
		*/
		//virtual ~HalfEdge() = default;

		/**
		* @brief Assignment Operation.
		* @param[in] other Another Edge.
		* @return Returns true if equal.
		*/
		bool operator==(const HalfEdge& other) const {
			return first == other.first && second == other.second;
		};

		/**
		* @brief Less Operation.
		* @param[in] other Another HalfEdge.
		* @return Returns true if less than other.
		*/
		bool operator<(const HalfEdge& other) const {
			return first < other.first;
		}
	};
}

namespace std {

	/**
	* @brief Hash function used for unordered_map.
	*/
	template<> struct hash<Spices::Edge> {
		size_t operator()(Spices::Edge const& edge) const {
			return ((hash<uint32_t>()(edge.first)  ^ (hash<uint32_t>()(edge.second) << 1)) >> 1)
				 + ((hash<uint32_t>()(edge.second) ^ (hash<uint32_t>()(edge.first)  << 1)) >> 1);
		}
	};

	/**
	* @brief Hash function used for unordered_map.
	*/
	template<> struct hash<Spices::HalfEdge> {
		size_t operator()(Spices::HalfEdge const& edge) const {
			return ((hash<uint32_t>()(edge.first) ^ (hash<uint32_t>()(edge.second) << 1)) >> 1);
		}
	};
}