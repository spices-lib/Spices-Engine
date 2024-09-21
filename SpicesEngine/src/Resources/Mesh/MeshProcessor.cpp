/**
* @file MeshProcessor.cpp.
* @brief The MeshProcessor Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "MeshProcessor.h"
#include "MeshPack.h"

#include <src/meshoptimizer.h>
#include <metis.h>
#include <glm/gtx/norm.hpp>

namespace Spices {

	void MeshProcessor::GenerateMeshLodClusterHierarchy(MeshPack* meshPack)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Create Lod0 meshlets.
		*/
		auto primVertices        = meshPack->m_MeshResource.primitiveVertices.attributes;
		meshPack->m_MeshResource.primitiveVertices.attributes = std::make_shared<std::vector<glm::uvec3>>();
		AppendMeshlets(meshPack, 0, *primVertices);

		uint32_t meshletStart = 0;
		const uint32_t maxLod = 25;
		for (uint32_t lod = 0; lod < maxLod; ++lod)
		{
			auto in = std::chrono::high_resolution_clock::now();
		
			float tLod = lod / (float)maxLod;
		
			/**
			* @brief meshlets of last lod.
			*/
			std::vector<Meshlet> meshlets = std::vector<Meshlet>(
				meshPack->m_MeshResource.meshlets.attributes->begin() + meshletStart, 
				meshPack->m_MeshResource.meshlets.attributes->end()
			);
		
			if (meshlets.size() <= 1)
			{
				return;
			}
		
			const uint32_t nextStart   = meshPack->m_MeshResource.meshlets.attributes->size();
			meshletStart               = nextStart;
			auto groups                = GroupMeshlets(meshPack, meshlets);
			
			for (const auto& group : groups)
			{
				std::vector<glm::uvec3> groupPrimVertices;
				auto ptr = meshPack->m_MeshResource.primitiveVertices.attributes;
				for (const auto& meshletIndex : group.meshlets)
				{
					const auto& meshlet = meshlets[meshletIndex];

					groupPrimVertices.insert(
						groupPrimVertices.end(), 
						ptr->begin() + meshlet.primitiveOffset, 
						ptr->begin() + meshlet.primitiveOffset + meshlet.nPrimitives
					);
				}

				/**
				* @brief Build KDTree.
				*/
				scl::kd_tree<6> kdTree;
				BuildKDTree(meshPack, groupPrimVertices, kdTree);

				std::vector<glm::vec3> points;
				PackVertexToPoints(meshPack, groupPrimVertices, points);

				float simplifyScale       = meshopt_simplifyScale(&points[0].x, points.size(), sizeof(glm::vec3));
				const float maxDistance   = (tLod * 0.01f + (1 - tLod) * 0.001f) * simplifyScale;
				const float maxUVDistance =  tLod * 0.1f  + (1 - tLod) * 0.001f;
				MergeByDistance(meshPack, groupPrimVertices, kdTree, maxDistance, maxUVDistance);

				/**
				* @brief Pack Sparse Inputs.
				*/
				std::vector<glm::vec3>                 packPoints;
				std::vector<glm::uvec3>                packPrimPoints;
				std::unordered_map<uint32_t, uint32_t> primVerticesMapReverse;
				PackVertexFromSparseInputs(meshPack, groupPrimVertices, packPoints, packPrimPoints, primVerticesMapReverse);

				/**
				* @brief Simplify meshlets group primPoints.
				*/
				const float threshold   = 0.5f;
				size_t targetCount      = packPrimPoints.size() * threshold * 3;
				float targetError       = 0.1f * tLod + 0.01f * (1 - tLod);
				uint32_t options        = meshopt_SimplifyLockBorder;
				
				std::vector<glm::uvec3> simplifiedPrimPoints(packPrimPoints.size());
				float simplificationError = 0.0f;
				
				size_t simplifiedCount = meshopt_simplify(
					&simplifiedPrimPoints[0].x ,
					&packPrimPoints[0].x       ,
					packPrimPoints.size() * 3  ,
					&packPoints[0].x           ,
					packPoints.size()          ,
					sizeof(glm::vec3)          ,
					targetCount                ,
					targetError                ,
					options                    ,
					&simplificationError
				);
				simplifiedPrimPoints.resize(simplifiedCount / 3);
				
				/**
				* @brief Simplify succeed: merge result to meshlets.
				*/
				std::vector<glm::uvec3> primVerticesBuffer;
				UnPackIndicesToSparseInputs(primVerticesBuffer, primVerticesMapReverse, simplifiedPrimPoints);
				
				AppendMeshlets(meshPack, lod + 1, primVerticesBuffer);
			}
		
			auto out = std::chrono::high_resolution_clock::now();
			std::cout << "    Lod Cost: " << std::chrono::duration_cast<std::chrono::milliseconds>(out - in).count() << "    " << meshlets.size() << std::endl;
		}
	}

	void MeshProcessor::AppendMeshlets(
		MeshPack*                      meshPack , 
		uint32_t                       lod      , 
		const std::vector<glm::uvec3>& primVertices
	)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief normal cone weight set 0.5f;
		* Get Const variable.
		*/
		const float coneWeight              = 0.5f;
		const uint32_t primLocationsOffset  = meshPack->m_MeshResource.primitiveLocations.attributes->size();
		const uint32_t primVerticesOffset   = meshPack->m_MeshResource.primitiveVertices.attributes->size();
		const uint32_t meshletsOffset       = meshPack->m_MeshResource.meshlets.attributes->size();
		
		/**
		* @brief Init meshopt variable.
		*/
		size_t max_meshlets = meshopt_buildMeshletsBound(primVertices.size() * 3, MESHLET_NVERTICES, MESHLET_NPRIMITIVES);
		std::vector<meshopt_Meshlet> meshoptlets(max_meshlets);
		std::vector<unsigned int> meshlet_vertices(max_meshlets * MESHLET_NVERTICES);
		std::vector<unsigned char> meshlet_triangles(max_meshlets * MESHLET_NPRIMITIVES * 3);
		
		/**
		* @brief Pack Sparse Inputs.
		*/
		std::vector<glm::vec3>  packPoints;
		std::vector<glm::uvec3> packPrimPoints;
		std::unordered_map<uint32_t, uint32_t> primVerticesMapReverse;
		PackVertexFromSparseInputs(meshPack, primVertices, packPoints, packPrimPoints, primVerticesMapReverse);

		/**
		* @brief Build Meshlets.
		*/
		size_t nMeshlet = meshopt_buildMeshlets(
			meshoptlets.data()         , 
			meshlet_vertices.data()    , 
			meshlet_triangles.data()   , 
			&packPrimPoints[0].x       ,
			packPrimPoints.size() * 3  ,
			&packPoints[0].x           ,
			packPoints.size()          ,
			sizeof(glm::vec3)          , 
			MESHLET_NVERTICES          , 
			MESHLET_NPRIMITIVES        , 
			coneWeight
		);
		
		/**
		* @brief Adjust meshopt variable.
		*/
		const meshopt_Meshlet& last = meshoptlets[nMeshlet - 1];
		meshoptlets      .resize(nMeshlet);
		meshlet_vertices .resize(last.vertex_offset + last.vertex_count);
		meshlet_triangles.resize(last.triangle_offset + (last.triangle_count * 3 + 3) & ~3);
		
		/**
		* @brief Optimize meshlets and compute meshlet bound and cone.
		*/
		uint32_t nPrimitives = 0;
		for (size_t i = 0; i < nMeshlet; ++i)
		{
			meshopt_optimizeMeshlet(
				&meshlet_vertices[meshoptlets[i].vertex_offset]            ,
				&meshlet_triangles[meshoptlets[i].triangle_offset]         ,
				meshoptlets[i].triangle_count, meshoptlets[i].vertex_count
			);
		
			const meshopt_Meshlet& m = meshoptlets[i];
			meshopt_Bounds bounds = meshopt_computeMeshletBounds(
				&meshlet_vertices[m.vertex_offset]    ,
				&meshlet_triangles[m.triangle_offset] ,
				m.triangle_count                      ,
				&packPoints[0].x,
				packPoints.size(),
				sizeof(glm::vec3)
			);
		
			Meshlet meshlet;
			meshlet.FromMeshopt(meshoptlets[i], bounds);
			meshlet.primitiveOffset = nPrimitives;
		
			meshlet.vertexOffset    += primLocationsOffset;
			meshlet.primitiveOffset += primVerticesOffset;
			meshlet.lod = lod;
		
			meshPack->m_MeshResource.meshlets.attributes->push_back(std::move(meshlet));
		
			nPrimitives += m.triangle_count;
		}

		/**
		* @brief Layout map for primpoints.
		*/
		std::unordered_map<glm::uvec3, uint32_t> inPrimPointsLayoutMap;
		auto& vertices = *meshPack->m_MeshResource.vertices.attributes;
		for (auto& primVertex : primVertices)
		{
			inPrimPointsLayoutMap[{ vertices[primVertex.x].x, vertices[primVertex.y].x, vertices[primVertex.z].x }] = inPrimPointsLayoutMap.size();
		}

		/**
		* @brief Fill in data back to meshpack variable.
		*/
		const Meshlet& lastm = (*meshPack->m_MeshResource.meshlets.attributes)[meshletsOffset + nMeshlet - 1];
		meshPack->m_MeshResource.primitivePoints    .attributes->resize(lastm.primitiveOffset + lastm.nPrimitives);
		meshPack->m_MeshResource.primitiveVertices  .attributes->resize(lastm.primitiveOffset + lastm.nPrimitives);
		meshPack->m_MeshResource.primitiveLocations .attributes->resize(lastm.primitiveOffset + lastm.nPrimitives);

		for (uint32_t i = 0; i < nMeshlet; i++)
		{
			const meshopt_Meshlet& m = meshoptlets[i];
			const Meshlet& ml = (*meshPack->m_MeshResource.meshlets.attributes)[meshletsOffset + i];
		
			for (uint32_t j = 0; j < m.triangle_count; j++)
			{
				uint32_t a = (uint32_t)meshlet_triangles[m.triangle_offset + 3 * j + 0] + m.vertex_offset;
				uint32_t b = (uint32_t)meshlet_triangles[m.triangle_offset + 3 * j + 1] + m.vertex_offset;
				uint32_t c = (uint32_t)meshlet_triangles[m.triangle_offset + 3 * j + 2] + m.vertex_offset;
		
				uint32_t& x = vertices[primVerticesMapReverse[meshlet_vertices[a]]].x;
				uint32_t& y = vertices[primVerticesMapReverse[meshlet_vertices[b]]].x;
				uint32_t& z = vertices[primVerticesMapReverse[meshlet_vertices[c]]].x;

				(*meshPack->m_MeshResource.primitivePoints.attributes)[ml.primitiveOffset + j] = { x, y, z };
		
				(*meshPack->m_MeshResource.primitiveLocations.attributes)[ml.primitiveOffset + j].x = a + primLocationsOffset;
				(*meshPack->m_MeshResource.primitiveLocations.attributes)[ml.primitiveOffset + j].y = b + primLocationsOffset;
				(*meshPack->m_MeshResource.primitiveLocations.attributes)[ml.primitiveOffset + j].z = c + primLocationsOffset;

				(*meshPack->m_MeshResource.primitiveVertices.attributes)[ml.primitiveOffset + j] = primVertices[inPrimPointsLayoutMap[{ x, y, z }]];
			}
		}
	}

	std::vector<MeshletGroup> MeshProcessor::GroupMeshlets(MeshPack* meshPack, std::vector<Meshlet>& meshlets)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Return one group with all meshlets.
		*/
		auto groupWithMeshlets = [&]() 
		{
			MeshletGroup group;
			for (int i = 0; i < meshlets.size(); ++i)
			{
				group.meshlets.push_back(i);
			}
			return std::vector{ group };
		};

		/**
		* @brief Return if less than 8 meshlets.
		*/
		if (meshlets.size() < 8)
		{
			return groupWithMeshlets();
		}

		std::unordered_map<Edge, std::set<size_t>> edges2Meshlets;
		std::unordered_map<size_t, std::vector<Edge>> meshlets2Edges;

		for (size_t meshletIndex = 0; meshletIndex < meshlets.size(); meshletIndex++)
		{
			const auto& meshlet = meshlets[meshletIndex];

			for (size_t triangleIndex = 0; triangleIndex < meshlet.nPrimitives; triangleIndex++)
			{
				const glm::uvec3& primPts = (*meshPack->m_MeshResource.primitivePoints.attributes)[meshlet.primitiveOffset + triangleIndex];

				Edge edge0{ primPts.x , primPts.y };
				Edge edge1{ primPts.y , primPts.z };
				Edge edge2{ primPts.z , primPts.x };

				edges2Meshlets[edge0].insert(meshletIndex);
				edges2Meshlets[edge1].insert(meshletIndex);
				edges2Meshlets[edge2].insert(meshletIndex);

				meshlets2Edges[meshletIndex].push_back(edge0);
				meshlets2Edges[meshletIndex].push_back(edge1);
				meshlets2Edges[meshletIndex].push_back(edge2);
			}
		}

		/*
		* @brief remove internal edge.
		*/
		for (auto first = edges2Meshlets.begin(), last = edges2Meshlets.end(); first != last;)
		{
			if ((*first).second.size() <= 1)
			{
				first = edges2Meshlets.erase(first);
			}
			else
			{
				++first;
			}
		}

		/*
		* @brief return if no connected edge.
		*/
		if (edges2Meshlets.empty())
		{
			return groupWithMeshlets();
		}

		idx_t vertexCount   = meshlets.size();
		idx_t ncon          = 1;
		idx_t nparts        = meshlets.size() / 4;
		assert(nparts > 1);

		idx_t options[METIS_NOPTIONS];
		METIS_SetDefaultOptions(options);

		options[METIS_OPTION_OBJTYPE] = METIS_OBJTYPE_CUT;
		options[METIS_OPTION_CCORDER] = 1;

		std::vector<idx_t> partition;
		partition.resize(vertexCount);

		/**
		* @brief xadj.
		*/
		std::vector<idx_t> xadjacency;
		xadjacency.reserve(vertexCount + 1);

		/**
		* @brief adjncy.
		* Stores Meshlets Index.
		*/
		std::vector<idx_t> edgeAdjacency;

		/**
		* @brief Connect meshlets Count of each meshlet.
		*/
		std::vector<idx_t> edgeWeights;

		for (size_t meshletIndex = 0; meshletIndex < meshlets.size(); meshletIndex++)
		{
			size_t startIndexInEdgeAdjacency = edgeAdjacency.size();
			for (const auto& edge : meshlets2Edges[meshletIndex])
			{
				/**
				* @brief Skip if edge is internal edge.
				*/
				auto connectionsIter = edges2Meshlets.find(edge);
				if (connectionsIter == edges2Meshlets.end())
				{
					continue;
				}

				/**
				* @brief Iter connected edge's meshlets.
				*/
				const auto& connections = connectionsIter->second;
				for (const auto& connectedMeshlet : connections)
				{
					/**
					* @brief Get a connect other meshlet.
					*/
					if (connectedMeshlet != meshletIndex)
					{
						auto existingEdgeIter = std::find(edgeAdjacency.begin() + startIndexInEdgeAdjacency, edgeAdjacency.end(), connectedMeshlet);
						if (existingEdgeIter == edgeAdjacency.end())
						{
							edgeAdjacency.emplace_back(connectedMeshlet);
							edgeWeights.emplace_back(1);
						}
						else
						{
							std::ptrdiff_t d = std::distance(edgeAdjacency.begin(), existingEdgeIter);
							assert(d >= 0 && d < edgeWeights.size());
							edgeWeights[d]++;
						}
					}
				}
			}
			xadjacency.push_back(startIndexInEdgeAdjacency);
		}
		xadjacency.push_back(edgeAdjacency.size());

		assert(xadjacency.size() == meshlets.size() + 1);
		assert(edgeAdjacency.size() == edgeWeights.size());

		/**
		* @brief Split Meshlets to Part.
		*/
		idx_t edgeCut;
		int result = METIS_PartGraphKway(
			&vertexCount,
			&ncon,
			xadjacency.data(),
			edgeAdjacency.data(),
			nullptr, /* vertex weights */
			nullptr, /* vertex size */
			edgeWeights.data(),
			&nparts,
			nullptr,
			nullptr,
			options,
			&edgeCut,
			partition.data()
		);

		assert(result == METIS_OK);

		/**
		* @brief Return groups.
		*/
		std::vector<MeshletGroup> groups;
		groups.resize(nparts);
		for (size_t i = 0; i < meshlets.size(); i++)
		{
			meshlets[i].clusterMeshletIndex = groups.size();

			idx_t partitionNumber = partition[i];
			groups[partitionNumber].meshlets.push_back(i);
		}

		return groups;
	}

	bool MeshProcessor::MergeByDistance(
		MeshPack*                meshPack      ,
		std::vector<glm::uvec3>& primVertices  ,
		scl::kd_tree<6>&         kdTree        , 
		float                    maxDistance   , 
		float                    maxUVDistance
	)
	{
		SPICES_PROFILE_ZONE;

		auto& positions = *meshPack->m_MeshResource.positions.attributes;
		auto& texCoords = *meshPack->m_MeshResource.texCoords.attributes;
		auto& vertices  = *meshPack->m_MeshResource.vertices .attributes;

		/**
		* @brief Get boundary points.
		*/
		std::unordered_map<uint32_t, bool>                               boundaryPoints;
		std::unordered_map<uint32_t, bool>                               stableBoundaryPoints;
		std::unordered_map<uint32_t, EdgePoint>                          boundaryEdgePoints;
		std::unordered_map<uint32_t, std::unordered_map<uint32_t, bool>> pointConnect;
		FindBoundaryPoints(
			meshPack             , 
			primVertices         , 
			boundaryPoints       , 
			stableBoundaryPoints , 
			boundaryEdgePoints   , 
			pointConnect
		);

		std::unordered_map<uint32_t, uint32_t> primVerticesMap;
		auto addToMap = [&](const uint32_t& k, const uint32_t& v) {
			if (primVerticesMap.find(k) == primVerticesMap.end())
			{
				primVerticesMap[k] = v;
			}
		};

		/**
		* @brief Find merged vertices.
		*/
		for (int i = 0; i < primVertices.size(); i++)
		{
			auto& primVertex = primVertices[i];
		
			std::array<uint32_t, 3> primVertexArray = { primVertex.x, primVertex.y, primVertex.z };
		
			for (int j = 0; j < 3; j++)
			{
				const glm::uvec4& vertex = vertices[primVertexArray[j]];
		
				/**
				* @brief Find near vertex.
				*/
				scl::kd_tree<6>::item item = { 
					positions[vertex.x].x, 
					positions[vertex.x].y, 
					positions[vertex.x].z, 
					texCoords[vertex.w].x, 
					texCoords[vertex.w].y, 
					(float)primVertexArray[j] 
				};
				auto rangeVts = kdTree.range_search(item, { 
					maxDistance   , 
					maxDistance   , 
					maxDistance   , 
					maxUVDistance , 
					maxUVDistance , 
					(float)UINT32_MAX 
				});
		
				/**
				* @brief Only allow near merge.
				*/
				std::vector<scl::kd_tree<6>::item> nearVts;
				for (auto& vt : rangeVts)
				{
					uint32_t pt = vertices[(uint32_t)vt[5]].x;
				
					if (pointConnect[vertex.x].find(pt) != pointConnect[vertex.x].end())
					{
						nearVts.push_back(vt);
					}
				}
		
				/**
				* @brief Can merge if this vertex is in stableboundary.
				* @attention This situation not allowed merge to line and point here.
				*/
				if (stableBoundaryPoints.find(vertex.x) != stableBoundaryPoints.end())
				{
					std::unordered_map<uint32_t, bool> mergedVertex;
		
					for (auto& rangeVt : nearVts)
					{
						uint32_t primVertexIndex = (uint32_t)rangeVt[5];
						mergedVertex[vertices[primVertexIndex].x] = true;
					}
		
					bool canBeTriangle = true;
					for (int k = 0; k < primVertexArray.size(); k++)
					{
						if (mergedVertex.find(vertices[primVertexArray[k]].x) != mergedVertex.end())
						{
							canBeTriangle = false;
							break;
						}
					}
		
					if(canBeTriangle)
					{ 
						for (auto& rangeVt : nearVts)
						{
							uint32_t primVertexIndex = (uint32_t)rangeVt[5];
							addToMap(primVertexIndex, primVertexArray[j]);
						}
					}
					else
					{
						addToMap(primVertexArray[j], primVertexArray[j]);
					}
				}
		
				/**
				* @brief Can merge when target is not in stableboundary if this vertex is in boundary.
				* @attention boundarypoint can only be merged when connected.
				*/
				else if (boundaryPoints.find(vertex.x) != boundaryPoints.end())
				{
					for (auto& rangeVt : nearVts)
					{
						uint32_t primVertexIndex = (uint32_t)rangeVt[5];
						uint32_t pt = vertices[primVertexIndex].x;
		
						if (boundaryPoints.find(pt) == boundaryPoints.end())
						{
							addToMap(primVertexIndex, primVertexArray[j]);
						}
						else if (stableBoundaryPoints.find(pt) == stableBoundaryPoints.end())
						{
							if (boundaryEdgePoints[vertex.x].next == pt ||
								boundaryEdgePoints[vertex.x].prev == pt ||
								boundaryEdgePoints[vertex.x].self == pt
							)
							{
								addToMap(primVertexIndex, primVertexArray[j]);
							}
						}
					}
				}
		
				/**
				* @brief Can merge when target is not in boundary if this vertex is not in boundary.
				*/
				else
				{
					for (auto& rangeVt : nearVts)
					{
						uint32_t primVertexIndex = (uint32_t)rangeVt[5];
						uint32_t pt = vertices[primVertexIndex].x;
		
						if (boundaryPoints.find(pt) == boundaryPoints.end())
						{
							addToMap(primVertexIndex, primVertexArray[j]);
						}
					}
				}
			}
		}

		/**
		* @brief Copy primVertices and merge.
		*/
		std::vector<glm::uvec3> tempPrimVertices = primVertices;
		for (int i = 0; i < primVertices.size(); i++)
		{
			auto primVertex = primVertices[i];
		
			if(primVerticesMap.find(primVertex.x) != primVerticesMap.end()) tempPrimVertices[i].x = primVerticesMap[primVertex.x];
			if(primVerticesMap.find(primVertex.y) != primVerticesMap.end()) tempPrimVertices[i].y = primVerticesMap[primVertex.y];
			if(primVerticesMap.find(primVertex.z) != primVerticesMap.end()) tempPrimVertices[i].z = primVerticesMap[primVertex.z];
		}
		
		/**
		* @brief Remove lines and poins merged from triangles;
		*/
		primVertices.clear();
		for (auto& primVertex : tempPrimVertices)
		{
			std::set<uint32_t> set;
			
			set.insert(primVertex.x);
			set.insert(primVertex.y);
			set.insert(primVertex.z);
			
			if (set.size() < 3) continue;
		
			primVertices.push_back(primVertex);
		}

		return true;
	}

	bool MeshProcessor::PackVertexToPoints(
		MeshPack*                      meshPack     , 
		const std::vector<glm::uvec3>& primVertices , 
		std::vector<glm::vec3>&        points
	)
	{
		SPICES_PROFILE_ZONE;

		std::unordered_map<uint32_t, bool> pointsMap;

		auto& positions = *meshPack->m_MeshResource.positions.attributes;
		auto& vertices  = *meshPack->m_MeshResource.vertices .attributes;
		for (auto& primVertex : primVertices)
		{
			const glm::uvec4& vertex0 = vertices[primVertex.x];
			const glm::uvec4& vertex1 = vertices[primVertex.y];
			const glm::uvec4& vertex2 = vertices[primVertex.z];

			pointsMap[vertex0.x] = true;
			pointsMap[vertex1.x] = true;
			pointsMap[vertex2.x] = true;
		}

		points.resize(pointsMap.size());
		uint32_t i = 0;
		for (auto& [pt, ignore] : pointsMap)
		{
			points[i] = positions[pt];
			++i;
		}

		return true;
	}

	bool MeshProcessor::BuildKDTree(
		MeshPack*                      meshPack     , 
		const std::vector<glm::uvec3>& primVertices ,
		scl::kd_tree<6>&               kdTree
	)
	{
		SPICES_PROFILE_ZONE;

		std::unordered_map<uint32_t, bool> primVerticesMap;
		for (auto& primVertex : primVertices)
		{
			primVerticesMap[primVertex.x] = true;
			primVerticesMap[primVertex.y] = true;
			primVerticesMap[primVertex.z] = true;
		}

		auto& positions = *meshPack->m_MeshResource.positions.attributes;
		auto& texCoords = *meshPack->m_MeshResource.texCoords.attributes;
		auto& vertices  = *meshPack->m_MeshResource.vertices .attributes;

		std::vector<scl::kd_tree<6>::item> items;
		items.resize(primVerticesMap.size());

		uint32_t i = 0;
		for (auto& [primVertex, ignore] : primVerticesMap)
		{
			glm::uvec4 vertex = vertices[primVertex];

			items[i] = 
			{ 
				positions[vertex.x].x, 
				positions[vertex.x].y, 
				positions[vertex.x].z, 
				texCoords[vertex.w].x, 
				texCoords[vertex.w].y, 
				(float)primVertex 
			};

			++i;
		}
		
		kdTree.insert(items);

		return true;
	}

	bool MeshProcessor::FindBoundaryPoints(
		MeshPack*                                                         meshPack             ,
		const std::vector<glm::uvec3>&                                    primVertices         ,
		std::unordered_map<uint32_t, bool>&                               boundaryPoints       ,
		std::unordered_map<uint32_t, bool>&                               stableBoundaryPoints ,
		std::unordered_map<uint32_t, EdgePoint>&                          boundaryEdgePoints   ,
		std::unordered_map<uint32_t, std::unordered_map<uint32_t, bool>>& pointConnect
	)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Get edge connected primitive.
		*/
		std::unordered_map<Edge, uint32_t> edgesConnects;
		auto& vertices = *meshPack->m_MeshResource.vertices.attributes;
		for (uint32_t triangleIndex = 0; triangleIndex < primVertices.size(); triangleIndex++)
		{
			auto& primVertex = primVertices[triangleIndex];
			std::array<uint32_t, 3> primVertexArray = { primVertex.x, primVertex.y, primVertex.z };

			for (uint32_t i = 0; i < 3; i++)
			{
				Edge edge;
				edge.first  = vertices[primVertexArray[i]].x;
				edge.second = vertices[primVertexArray[(i + 1) % 3]].x;

				if (edgesConnects.find(edge) == edgesConnects.end())
				{
					edgesConnects[edge] = 1;
				}
				else
				{
					edgesConnects[edge]++;
				}
			}
		}

		/**
		* @brief Get pointconnect.
		*/
		for (auto& [edge, connects] : edgesConnects)
		{
			pointConnect[edge.first][edge.second] = true;
			pointConnect[edge.first][edge.first]  = true;

			pointConnect[edge.second][edge.first]  = true;
			pointConnect[edge.second][edge.second] = true;
		}

		/*
		* @brief remove internal edge.
		*/
		for (auto first = edgesConnects.begin(), last = edgesConnects.end(); first != last;)
		{
			if ((*first).second != 1)
			{
				first = edgesConnects.erase(first);
			}
			else
			{
				++first;
			}
		}

		for (auto& [edge, connects] : edgesConnects)
		{
			boundaryPoints[edge.first]  = true;
			boundaryPoints[edge.second] = true;
		}

		/**
		* @brief get bound edge points.
		*/
		for (auto& [edge, connects] : edgesConnects)
		{
			if (boundaryEdgePoints.find(edge.first) == boundaryEdgePoints.end())
			{
				boundaryEdgePoints[edge.first].self = edge.first;
				boundaryEdgePoints[edge.first].next = edge.second;
			}
			else
			{
				boundaryEdgePoints[edge.first].prev = edge.second;
			}

			if (boundaryEdgePoints.find(edge.second) == boundaryEdgePoints.end())
			{
				boundaryEdgePoints[edge.second].self = edge.second;
				boundaryEdgePoints[edge.second].next = edge.first;
			}
			else
			{
				boundaryEdgePoints[edge.second].prev = edge.first;
			}
		}

		/**
		* @brief only stable points with high curvature.
		*/
		auto& position = *meshPack->m_MeshResource.positions.attributes;
		for (auto& pair : boundaryEdgePoints)
		{
			if (!pair.second.valid()) continue;

			glm::vec3 l = glm::normalize(position[pair.second.next] - position[pair.first]);
			glm::vec3 r = glm::normalize(position[pair.second.prev] - position[pair.first]);
			
			if (glm::dot(l, r) > -0.98f)
			{
				stableBoundaryPoints[pair.second.self] = true;

#define ExpandStablePoint
#ifdef ExpandStablePoint

				stableBoundaryPoints[pair.second.next] = true;
				stableBoundaryPoints[pair.second.prev] = true;

#endif

			}
		}

		return true;
	}

	bool MeshProcessor::PackVertexFromSparseInputs(
		MeshPack*                               meshPack              , 
		const std::vector<glm::uvec3>           primVertices          ,
		std::vector<glm::vec3>&                 packPoints            ,
		std::vector<glm::uvec3>&                packPrimPoints        ,
		std::unordered_map<uint32_t, uint32_t>& primVerticesMapReverse
	)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Merge Vertex.
		*/
		std::unordered_map<uint32_t, uint32_t> primVerticesMap;   // old: primVertexIndex  , new: primPointIndex
		for (auto& primVertex : primVertices)
		{
			if (primVerticesMap.find(primVertex.x) == primVerticesMap.end()) primVerticesMap[primVertex.x] = primVerticesMap.size();
			if (primVerticesMap.find(primVertex.y) == primVerticesMap.end()) primVerticesMap[primVertex.y] = primVerticesMap.size();
			if (primVerticesMap.find(primVertex.z) == primVerticesMap.end()) primVerticesMap[primVertex.z] = primVerticesMap.size();
		}
		
		/**
		* @brief Flatten Vertex Position.
		*/
		packPoints.resize(primVerticesMap.size());
		auto& vertices = *meshPack->m_MeshResource.vertices.attributes;
		for (auto& pair : primVerticesMap)
		{
			packPoints[pair.second] = (*meshPack->m_MeshResource.positions.attributes)[vertices[pair.first].x];
			primVerticesMapReverse[pair.second] = pair.first;
		}
		
		packPrimPoints.resize(primVertices.size());
		for (int i = 0; i < primVertices.size(); i++)
		{
			glm::uvec3 primVertex = primVertices[i];
		
			packPrimPoints[i].x = primVerticesMap[primVertex.x];
			packPrimPoints[i].y = primVerticesMap[primVertex.y];
			packPrimPoints[i].z = primVerticesMap[primVertex.z];
		}

		return true;
	}

	bool MeshProcessor::UnPackIndicesToSparseInputs(
		std::vector<glm::uvec3>&                primVertices           , 
		std::unordered_map<uint32_t, uint32_t>& primVerticesMapReverse ,
		const std::vector<glm::uvec3>&          packPrimPoints
	)
	{
		SPICES_PROFILE_ZONE;

		primVertices.resize(packPrimPoints.size());
		for (int i = 0; i < packPrimPoints.size(); i++)
		{
			uint32_t x = primVerticesMapReverse[packPrimPoints[i].x];
			uint32_t y = primVerticesMapReverse[packPrimPoints[i].y];
			uint32_t z = primVerticesMapReverse[packPrimPoints[i].z];

			primVertices[i] = glm::uvec3(x, y, z);
		}

		return true;
	}
}