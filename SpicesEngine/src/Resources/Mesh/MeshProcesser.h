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

	class MeshPack;

	/**
	* @brief Class for provide functions of process Meshpack.
	*/
	class MeshProcesser
	{
	public:

		/**
		* @brief Create Meshlets from MeshPack vertices and indices.
		*/
		static void CreateMeshlets(MeshPack* meshPack);

		/**
		* @brief Split Meshlets to Groups.
		* @param[in] indices MeshPack Indices.
		* @param[in] meshlets MeshPack Meshlets.
		* @return Returns Groups.
		*/
		static std::vector<MeshletGroup> GroupMeshlets(const std::vector<uint32_t>& indices, const std::vector<Meshlet>& meshlets);

		static void Samplify(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, std::vector<MeshletGroup>& groups, std::vector<Meshlet>& meshlets);

		static void GenerateMeshLodClusterHierarchy(MeshPack* meshPack);

		static void AppendMeshlets(MeshPack* meshPack, const std::vector<uint32_t> indexBuffer);
	};
}