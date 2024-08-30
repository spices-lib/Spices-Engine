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
		* @brief Generate Mesh Lod Resources.
		* @param[in] meshPack MeshPack.
		*/
		static void GenerateMeshLodClusterHierarchy(MeshPack* meshPack);

	private:

		/**
		* @brief Create and Append Meshlets to MeshPack use given indices.
		* @param[in] meshPack MeshPack.
		* @param[in] indices Indices Buffer.
		*/
		static void AppendMeshlets(MeshPack* meshPack, const std::vector<uint32_t> indices);

		/**
		* @brief Split Meshlets to Groups.
		* @param[in] meshPack MeshPack.
		* @param[in] meshlets MeshPack Meshlets.
		* @return Returns Groups.
		*/
		static std::vector<MeshletGroup> GroupMeshlets(MeshPack* meshPack, const std::vector<Meshlet>& meshlets);
	};
}