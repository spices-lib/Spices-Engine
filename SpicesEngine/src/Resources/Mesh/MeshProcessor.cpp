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
		uint32_t meshletStart    = 0;
		auto primPoints          = meshPack->m_MeshResource.primitivePoints.attributes;
		auto primVertices        = meshPack->m_MeshResource.primitiveVertices.attributes;
		meshPack->m_MeshResource.primitivePoints  .attributes = std::make_shared<std::vector<glm::uvec3>>();
		meshPack->m_MeshResource.primitiveVertices.attributes = std::make_shared<std::vector<glm::uvec3>>();
		AppendMeshlets(meshPack, 0, *primVertices);

		const uint32_t maxLod = 1;
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
				for (const auto& meshletIndex : group.meshlets)
				{
					const auto& meshlet = meshlets[meshletIndex];

					auto ptr = meshPack->m_MeshResource.primitiveVertices.attributes;
					groupPrimVertices.insert(groupPrimVertices.end(), ptr->begin() + meshlet.primitiveOffset, ptr->begin() + meshlet.primitiveOffset + meshlet.nPrimitives);
				}

				/**
				* @brief Build KDTree.
				*/
				scl::kd_tree<6> kdTree;
				BuildKDTree(meshPack, groupPrimVertices, kdTree);

				//float simplifyScale          = meshopt_simplifyScale(&packPoints[0].x, packPrimPoints.size() * 3, sizeof(glm::vec3));
				const float maxDistance   = (tLod * 0.1f + (1 - tLod) * 0.01f);
				const float maxUVDistance = tLod * 0.5f + (1 - tLod) * 1.0f / 256.0f;
				auto primVerticesMap      = MergeByDistance(meshPack, groupPrimVertices, kdTree, maxDistance, maxUVDistance);

				FindAndStableBoundaryVertices(meshPack, groupPrimVertices, primVerticesMap);

				/**
				* @brief Pack Sparse Inputs.
				*/
				std::vector<glm::vec3>  packPoints;
				std::vector<glm::uvec4> packVertices;
				std::vector<glm::uvec3> packPrimPoints;
				std::vector<glm::uvec3> packPrimVertices;
				std::unordered_map<uint32_t, uint32_t> primPointsMapReverse;
				std::unordered_map<uint32_t, uint32_t> primVerticesMapReverse;
				PackVertexFromSparseInputs(meshPack, groupPrimVertices, packPoints, packPrimPoints, packPrimVertices, primPointsMapReverse, primVerticesMapReverse);
				
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
				UnPackIndicesToSparseInputs(primVerticesBuffer, primPointsMapReverse, simplifiedPrimPoints);
				
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
		std::vector<glm::uvec3> packPrimVertices;
		std::unordered_map<uint32_t, uint32_t> primPointsMapReverse;
		std::unordered_map<uint32_t, uint32_t> primVerticesMapReverse;
		PackVertexFromSparseInputs(meshPack, primVertices, packPoints, packPrimPoints, packPrimVertices, primPointsMapReverse, primVerticesMapReverse);
		
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
		meshoptlets.resize(nMeshlet);
		meshlet_vertices.resize(last.vertex_offset + last.vertex_count);
		meshlet_triangles.resize(last.triangle_offset + (last.triangle_count * 3 + 3) & ~3);
		
		/**
		* @brief Optimize meshlets and compute meshlet bound and cone.
		*/
		uint32_t nPrimitives = 0;
		{
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
					&packPoints[0].x                      ,
					packPoints.size()                     ,
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
		}
		
		/**
		* @brief Layout map for primpoints.
		*/
		std::unordered_map<glm::uvec3, uint32_t> inPrimPointsLayoutMap;
		for (auto& primVertex : primVertices)
		{
			auto& vertices = *meshPack->m_MeshResource.vertices.attributes;
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
		
				uint32_t& x = primPointsMapReverse[meshlet_vertices[a]];
				uint32_t& y = primPointsMapReverse[meshlet_vertices[b]];
				uint32_t& z = primPointsMapReverse[meshlet_vertices[c]];

				(*meshPack->m_MeshResource.primitivePoints.attributes)[ml.primitiveOffset + j] = { x, y, z };
		
				(*meshPack->m_MeshResource.primitiveLocations.attributes)[ml.primitiveOffset + j].x = a + primLocationsOffset;
				(*meshPack->m_MeshResource.primitiveLocations.attributes)[ml.primitiveOffset + j].y = b + primLocationsOffset;
				(*meshPack->m_MeshResource.primitiveLocations.attributes)[ml.primitiveOffset + j].z = c + primLocationsOffset;

				(*meshPack->m_MeshResource.primitiveVertices.attributes)[ml.primitiveOffset + j] = primVertices[inPrimPointsLayoutMap[{ x, y, z }]];
			}
		}
	}

	std::vector<MeshletGroup> MeshProcessor::GroupMeshlets(MeshPack* meshPack, const std::vector<Meshlet>& meshlets)
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
			idx_t partitionNumber = partition[i];
			groups[partitionNumber].meshlets.push_back(i);
		}

		return groups;
	}

	std::unordered_map<uint32_t, uint32_t> MeshProcessor::MergeByDistance(
		MeshPack*                      meshPack      ,
		const std::vector<glm::uvec3>& primVertices  ,
		scl::kd_tree<6>&               kdTree        , 
		float                          maxDistance   , 
		float                          maxUVDistance
	)
	{
		SPICES_PROFILE_ZONE;

		std::unordered_map<uint32_t, uint32_t> primVerticesMap;

		auto& positions = *meshPack->m_MeshResource.positions.attributes;
		auto& texCoords = *meshPack->m_MeshResource.texCoords.attributes;
		auto& vertices  = *meshPack->m_MeshResource.vertices .attributes;

		for (int i = 0; i < primVertices.size(); i++)
		{
			auto& primVertex = primVertices[i];

			std::array<uint32_t, 3> primVertexArray = { primVertex.x, primVertex.y, primVertex.z };

			for (int j = 0; j < primVertexArray.size(); j++)
			{
				glm::uvec4 vertex = vertices[primVertexArray[j]];
				scl::kd_tree<6>::item item = { positions[vertex.x].x, positions[vertex.x].y, positions[vertex.x].z, texCoords[vertex.w].x, texCoords[vertex.w].y, (float)primVertexArray[j] };
				auto rangeVts = kdTree.range_search(item, { maxDistance, maxDistance, maxDistance, maxUVDistance, maxUVDistance, (float)UINT32_MAX });

				for (auto& rangeVt : rangeVts)
				{
					uint32_t primVertexIndex = (uint32_t)rangeVt[5];

					if (primVerticesMap.find(primVertexIndex) == primVerticesMap.end())
					{
						primVerticesMap[primVertexIndex] = primVertexArray[j];
					}
				}
			}
		}

		return primVerticesMap;
	}

	bool MeshProcessor::BuildKDTree(
		MeshPack*                      meshPack     , 
		const std::vector<glm::uvec3>& primVertices ,
		scl::kd_tree<6>&               kdTree
	)
	{
		SPICES_PROFILE_ZONE;

		std::vector<scl::kd_tree<6>::item> items;
		items.resize(primVertices.size() * 3);

		auto& positions = *meshPack->m_MeshResource.positions.attributes;
		auto& texCoords = *meshPack->m_MeshResource.texCoords.attributes;
		auto& vertices  = *meshPack->m_MeshResource.vertices.attributes;
		for (int i = 0; i < primVertices.size(); i++)
		{
			auto& primVertex = primVertices[i];

			glm::uvec4 vertex0 = vertices[primVertex.x];
			glm::uvec4 vertex1 = vertices[primVertex.y];
			glm::uvec4 vertex2 = vertices[primVertex.z];

			items[3 * i + 0] = { positions[vertex0.x].x, positions[vertex0.x].y, positions[vertex0.x].z, texCoords[vertex0.w].x, texCoords[vertex0.w].y, (float)primVertex.x };
			items[3 * i + 1] = { positions[vertex1.x].x, positions[vertex1.x].y, positions[vertex1.x].z, texCoords[vertex1.w].x, texCoords[vertex1.w].y, (float)primVertex.y };
			items[3 * i + 2] = { positions[vertex2.x].x, positions[vertex2.x].y, positions[vertex2.x].z, texCoords[vertex2.w].x, texCoords[vertex2.w].y, (float)primVertex.z };
		}
		
		kdTree.insert(items);

		return true;
	}

	bool MeshProcessor::FindAndStableBoundaryVertices(
		MeshPack*                               meshPack     ,
		std::vector<glm::uvec3>&                primVertices ,
		std::unordered_map<uint32_t, uint32_t>& primVerticesMap
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

#if 0

		for (auto& [edge, connects] : edgesConnects)
		{
			if (connects == 1)
			{
				boundary[edge.first] = true;
				boundary[edge.second] = true;
			}
		}

#else // Allow facet points in boundary fuse, not working correct now.

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

		struct EdgePoint
		{
			EdgePoint() 
				: previous(-1)
				, self(-1)
				, next(-1)
			{}

			uint32_t previous;
			uint32_t self;
			uint32_t next;

			bool valid()
			{
				return previous != -1 && self != -1 && next != -1;
			}
		};

		/**
		* @brief get bound edge points.
		*/
		std::unordered_map<uint32_t, EdgePoint> boundEdgePoints;
		for (auto& pair : edgesConnects)
		{
			if (boundEdgePoints.find(pair.first.first) == boundEdgePoints.end())
			{
				boundEdgePoints[pair.first.first].self = pair.first.first;
				boundEdgePoints[pair.first.first].next = pair.first.second;
			}
			else
			{
				boundEdgePoints[pair.first.first].previous = pair.first.second;
			}

			if (boundEdgePoints.find(pair.first.second) == boundEdgePoints.end())
			{
				boundEdgePoints[pair.first.second].self = pair.first.second;
				boundEdgePoints[pair.first.second].next = pair.first.first;
			}
			else
			{
				boundEdgePoints[pair.first.second].previous = pair.first.first;
			}
		}

		/**
		* @brief only stable points with high curvature.
		*/
		auto& position = *meshPack->m_MeshResource.positions.attributes;
		std::unordered_map<uint32_t, bool> boundary;
		for (auto& pair : boundEdgePoints)
		{
			if (!pair.second.valid()) continue;

			glm::vec3 l = glm::normalize(position[pair.second.next]     - position[pair.first]);
			glm::vec3 r = glm::normalize(position[pair.second.previous] - position[pair.first]);
			
			if (glm::dot(l, r) > -0.98f)
			{
				boundary[pair.first] = true;
			}
		}

		for (int i = 0; i < primVertices.size(); i++)
		{
			auto& primVertex = primVertices[i];

			if(boundary.find(vertices[primVertex.x].x) == boundary.end()) primVertices[i].x = primVerticesMap[primVertex.x];
			if(boundary.find(vertices[primVertex.y].x) == boundary.end()) primVertices[i].y = primVerticesMap[primVertex.y];
			if(boundary.find(vertices[primVertex.z].x) == boundary.end()) primVertices[i].z = primVerticesMap[primVertex.z];
		}

#endif

		return true;
	}

	bool MeshProcessor::PackVertexFromSparseInputs(
		MeshPack*                               meshPack              , 
		const std::vector<glm::uvec3>           primVertices          ,
		std::vector<glm::vec3>&                 packPoints            ,
		std::vector<glm::uvec3>&                packPrimPoints        ,
		std::vector<glm::uvec3>&                packPrimVertices      ,
		std::unordered_map<uint32_t, uint32_t>& primPointsMapReverse  ,
		std::unordered_map<uint32_t, uint32_t>& primVerticesMapReverse
	)
	{
		SPICES_PROFILE_ZONE;

		std::unordered_map<uint32_t, uint32_t> primVerticesMap;  // key: original vertex index, value: new vertex index.
		for (const auto& primVertex : primVertices)
		{
			if (primVerticesMap.find(primVertex.x) == primVerticesMap.end()) primVerticesMap[primVertex.x] = primVerticesMap.size();
			if (primVerticesMap.find(primVertex.y) == primVerticesMap.end()) primVerticesMap[primVertex.y] = primVerticesMap.size();
			if (primVerticesMap.find(primVertex.z) == primVerticesMap.end()) primVerticesMap[primVertex.z] = primVerticesMap.size();
		}

		std::unordered_map<uint32_t, uint32_t> primPointsMap;    // key: original point index, value: new point index.
		for (const auto& pair : primVerticesMap)
		{
			auto& vertices = *meshPack->m_MeshResource.vertices.attributes;

			if (primPointsMap.find(vertices[pair.first].x) == primPointsMap.end()) primPointsMap[vertices[pair.first].x] = primPointsMap.size();
			primVerticesMapReverse[pair.second] = pair.first;
		}

		packPoints.resize(primPointsMap.size());
		for (auto& pair : primPointsMap)
		{
			packPoints[pair.second] = (*meshPack->m_MeshResource.positions.attributes)[pair.first];
			primPointsMapReverse[pair.second] = pair.first;
		}

		packPrimPoints.resize(primVertices.size());
		for (int i = 0; i < primVertices.size(); i++)
		{
			glm::uvec3 primVertex = primVertices[i];
			auto& vertices = *meshPack->m_MeshResource.vertices.attributes;

			packPrimPoints[i].x = primPointsMap[vertices[primVertex.x].x];
			packPrimPoints[i].y = primPointsMap[vertices[primVertex.y].x];
			packPrimPoints[i].z = primPointsMap[vertices[primVertex.z].x];
		}

		packPrimVertices.resize(primVertices.size());
		for (int i = 0; i < primVertices.size(); i++)
		{
			glm::uvec3 primVertex = primVertices[i];

			packPrimVertices[i].x = primVerticesMap[primVertex.x];
			packPrimVertices[i].y = primVerticesMap[primVertex.y];
			packPrimVertices[i].z = primVerticesMap[primVertex.z];
		}

		return true;
	}

	bool MeshProcessor::UnPackIndicesToSparseInputs(
		std::vector<glm::uvec3>&                primVertices         , 
		std::unordered_map<uint32_t, uint32_t>& primPointsMapReverse , 
		const std::vector<glm::uvec3>&          packPrimPoints
	)
	{
		SPICES_PROFILE_ZONE;

		primVertices.resize(packPrimPoints.size());
		for (int i = 0; i < packPrimPoints.size(); i++)
		{
			uint32_t x = primPointsMapReverse[packPrimPoints[i].x];
			uint32_t y = primPointsMapReverse[packPrimPoints[i].y];
			uint32_t z = primPointsMapReverse[packPrimPoints[i].z];

			glm::uvec3 primPoint = glm::uvec3(x, y, z);

		}

		return true;
	}
}