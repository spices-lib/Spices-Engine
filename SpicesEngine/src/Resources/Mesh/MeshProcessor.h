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

	/**
	* @brief MeshletGroup for simplify.
	*/
	struct MeshletGroup 
	{
		std::vector<size_t> meshlets;
	};

	/**
	* @brief Forward declear.
	*/
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
		* @param[in] clusterBoundSphere Cluster's Bound Sphere.
		* @param[in] primVertices PrimVertices Buffer.
		*/
		static void AppendMeshlets(
			MeshPack*                      meshPack           , 
			uint32_t                       lod                , 
			const SpicesShader::Sphere&    clusterBoundSphere ,
			const std::vector<glm::uvec3>& primVertices
		);

		/**
		* @brief Split Meshlets to Groups.
		* @param[in] meshPack MeshPack.
		* @param[in] meshlets MeshPack Meshlets.
		* @return Returns Groups.
		*/
		static std::vector<MeshletGroup> GroupMeshlets(
			MeshPack*             meshPack , 
			std::vector<Meshlet>& meshlets
		);

		/**
		* @brief Merge Vertex by Distance.
		* @param[in] meshPack MeshPack.
		* @param[in] kdTree kd_tree.
		* @param[in] maxDistance allowed merge by vertex position.
		* @param[in] maxUVDistance allowed merge by vertex uv.
		* @return Returns Merged Vertex Map.
		*/
		static bool MergeByDistance(
			MeshPack*                meshPack      ,
			std::vector<glm::uvec3>& primVertices  ,
			scl::kd_tree<6>&         kdTree        , 
			float                    maxDistance   , 
			float                    maxUVDistance
		);

		/**
		* @brief Pack Vertices to Points.
		* @param[in] meshPack MeshPack.
		* @param[in] primVertices .
		* @param[in,out] points .
		* @return Returns true if succeed.
		*/
		static bool PackVertexToPoints(
			MeshPack*                      meshPack      ,
			const std::vector<glm::uvec3>& primVertices  ,
			std::vector<glm::vec3>&        points
		);

		/**
		* @brief Calculate BoundSphere from Points.
		* @param[in] points .
		* @return Returns BoundSphere.
		*/
		static SpicesShader::Sphere CalculateBoundSphere(
			const std::vector<glm::vec3>& points
		);

		/**
		* @brief Build KDTree use specific meshlets.
		* @param[in] meshPack MeshPack.
		* @param[in] primVertices PrimVertices.
		* @param[in,out] kdTree .
		* @return Returns true if succeed.
		*/
		static bool BuildKDTree(
			MeshPack*                      meshPack     ,
			const std::vector<glm::uvec3>& primVertices ,
			scl::kd_tree<6>&               kdTree
		);

		/**
		* @brief Find Points located in Boundarys.
		* @param[in] meshPack MeshPack.
		* @param[in] primVertices .
		* @param[in,out] boundaryPoints .
		* @param[in,out] stableBoundaryPoints .
		* @param[in,out] boundaryEdgePoints .
		* @param[in,out] pointConnect .
		* @return Returns true if succeed.
		*/
		static bool FindBoundaryPoints(
			MeshPack*                                                         meshPack             ,
			const std::vector<glm::uvec3>&                                    primVertices         ,
			std::unordered_map<uint32_t, bool>&                               boundaryPoints       ,
			std::unordered_map<uint32_t, bool>&                               stableBoundaryPoints ,
			std::unordered_map<uint32_t, EdgePoint>&                          boundaryEdgePoints   ,
			std::unordered_map<uint32_t, std::unordered_map<uint32_t, bool>>& pointConnect
		);

		/**
		* @brief Pack PrimVertices from Sparse PrimVertices Inputs.
		* @param[in] meshPack MeshPack.
		* @param[in] primVertices .
		* @param[in,out] packPoints .
		* @param[in,out] packPrimPoints ,
		* @param[in,out] primVerticesMapReverse .
		* @return Returns true if succeed.
		*/
		static bool PackPrimVerticesFromSparseInputs(
			MeshPack*                               meshPack              , 
			const std::vector<glm::uvec3>           primVertices          ,
			std::vector<glm::vec3>&                 packPoints            ,
			std::vector<glm::uvec3>&                packPrimPoints        ,
			std::unordered_map<uint32_t, uint32_t>& primVerticesMapReverse
		);

		/**
		* @brief Unpack PrimVertices to Sparse PrimVertices Inputs.
		* @param[in,out] primVertices .
		* @param[in] primVerticesMapReverse .
		* @param[in] packPrimPoints .
		* @return Returns true if succeed.
		*/
		static bool UnPackPrimVerticesToSparseInputs(
			std::vector<glm::uvec3>&                primVertices           , 
			std::unordered_map<uint32_t, uint32_t>& primVerticesMapReverse ,
			const std::vector<glm::uvec3>&          packPrimPoints
		);
	};
}