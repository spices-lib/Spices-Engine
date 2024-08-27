/**
* @file MeshProcesser.h.
* @brief The MeshProcesser Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Vertex.h"

namespace Spices {

	struct MeshletGroup 
	{
		std::vector<size_t> meshlets;
	};

	/**
	* @brief Class for provide functions of process Meshpack.
	*/
	class MeshProcesser
	{
	public:

		/**
		* @brief Create Meshlets from MeshPack vertices and indices.
		* @param[in] vertices MeshPack vertices.
		* @param[in] indices MeshPack indices.
		* @param[in] meshlets MeshPack meshlets.
		*/
		static void CreateMeshlets(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, std::vector<Meshlet>& meshlets);

		/**
		* @brief Split Meshlets to Groups.
		* @param[in] indices MeshPack Indices.
		* @param[in] meshlets MeshPack Meshlets.
		* @return Returns Groups.
		*/
		static std::vector<MeshletGroup> GroupMeshlets(const std::vector<uint32_t>& indices, const std::vector<Meshlet>& meshlets);
	};
}