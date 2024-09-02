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
		* @param[in] lod current lod level.
		* @param[in] indices Indices Buffer.
		*/
		static void AppendMeshlets(MeshPack* meshPack, uint32_t lod, const std::vector<uint32_t> indices);

		/**
		* @brief Split Meshlets to Groups.
		* @param[in] meshPack MeshPack.
		* @param[in] meshlets MeshPack Meshlets.
		* @return Returns Groups.
		*/
		static std::vector<MeshletGroup> GroupMeshlets(MeshPack* meshPack, const std::vector<Meshlet>& meshlets);

		/**
		* @brief Merge Vertex by Distance.
		* @param[in] meshPack MeshPack.
		* @param[in] maxDistance allowed merge by vertex position.
		* @param[in] maxUVDistance allowed merge by vertex uv.
		* @return Returns Merged Vertex Map.
		*/
		static std::vector<uint64_t> MergeByDistance(MeshPack* meshPack, float maxDistance, float maxUVDistance);
	};
}