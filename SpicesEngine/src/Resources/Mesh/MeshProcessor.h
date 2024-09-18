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
		* @param[in] primVertices PrimVertices Buffer.
		*/
		static void AppendMeshlets(
			MeshPack*                      meshPack , 
			uint32_t                       lod      , 
			const std::vector<glm::uvec3>& primVertices
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
		static std::unordered_map<uint32_t, uint32_t> MergeByDistance(
			MeshPack*                      meshPack      ,
			const std::vector<glm::uvec3>& primVertices  ,
			scl::kd_tree<6>&               kdTree        , 
			float                          maxDistance   , 
			float                          maxUVDistance
		);

		/**
		* @brief Build KDTree use specific meshlets.
		* @param[in] meshPack MeshPack.
		* @param[in] primVertices PrimVertices.
		* @param[in] kdTree .
		*/
		static bool BuildKDTree(
			MeshPack*                      meshPack     ,
			const std::vector<glm::uvec3>& primVertices ,
			scl::kd_tree<6>&               kdTree
		);

		static bool FindAndStableBoundaryVertices(
			MeshPack*                               meshPack       ,
			std::vector<glm::uvec3>&                primVertices   ,
			std::unordered_map<uint32_t, uint32_t>& primVerticesMap
		);

		static bool PackVertexFromSparseInputs(
			MeshPack*                               meshPack              , 
			const std::vector<glm::uvec3>           primVertices          ,
			std::vector<glm::vec3>&                 packPoints            ,
			std::vector<glm::uvec3>&                packPrimPoints        ,
			std::unordered_map<uint32_t, uint32_t>& primPointsMapReverse
		);

		static bool UnPackIndicesToSparseInputs(
			std::vector<glm::uvec3>&                primVertices         , 
			std::unordered_map<uint32_t, uint32_t>& primPointsMapReverse , 
			const std::vector<glm::uvec3>&          packPrimPoints
		);
	};
}