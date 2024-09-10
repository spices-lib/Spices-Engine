/**
* @file MeshProcessor.h.
* @brief The MeshProcessor Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Vertex.h"
#include "Core/Container/kd_tree.h"

namespace Spices {

	struct MeshletGroup 
	{
		std::vector<size_t> meshlets;
	};

	class MeshPack;

	/**
	* @brief Class for provide functions of process Meshpack.
	*/
	class MeshProcessor
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
		static void AppendMeshlets(
			MeshPack*                    meshPack , 
			uint32_t                     lod      , 
			const std::vector<uint32_t>& indices
		);

		/**
		* @brief Split Meshlets to Groups.
		* @param[in] meshPack MeshPack.
		* @param[in] meshlets MeshPack Meshlets.
		* @return Returns Groups.
		*/
		static std::vector<MeshletGroup> GroupMeshlets(
			MeshPack*                   meshPack , 
			const std::vector<Meshlet>& meshlets
		);

		/**
		* @brief Merge Vertex by Distance.
		* @param[in] meshPack MeshPack.
		* @param[in] kdTree kd_tree.
		* @param[in] maxDistance allowed merge by vertex position.
		* @param[in] maxUVDistance allowed merge by vertex uv.
		* @return Returns Merged Vertex Map.
		*/
		static std::vector<uint32_t> MergeByDistance(
			const std::vector<Vertex>& vertices      , 
			const std::vector<bool>&   boundary      , 
			scl::kd_tree<6>&           kdTree        , 
			float                      maxDistance   , 
			float                      maxUVDistance
		);

		/**
		* @brief Build KDTree use specific meshlets.
		* @param[in] meshPack MeshPack.
		* @param[in] kdTree .
		*/
		static bool BuildKDTree(
			const std::vector<Vertex>& vertices, 
			scl::kd_tree<6>& kdTree
		);

		static bool FindBoundaryVertices(
			const std::vector<uint32_t>& indices  , 
			std::vector<bool>&           boundary
		);

		static bool PackVertexFromSparseInputs(
			MeshPack*                               meshPack          , 
			const std::vector<uint32_t>             indices           , 
			std::vector<Vertex>&                    packVertices      , 
			std::vector<uint32_t>&                  packIndices       , 
			std::unordered_map<uint32_t, uint32_t>& indicesMapReverse
		);

		static bool UnPackIndicesToSparseInputs(
			std::vector<uint32_t>&                  indices           , 
			std::unordered_map<uint32_t, uint32_t>& indicesMapReverse , 
			const std::vector<uint32_t>&            packIndices
		);
	};
}