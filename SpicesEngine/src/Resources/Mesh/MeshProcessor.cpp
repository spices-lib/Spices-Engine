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
		meshPack->m_MeshResource.primitivePoints.attributes = std::make_shared<std::vector<glm::ivec3>>();
		AppendMeshlets(meshPack, 0, *primPoints);

		const int maxLod = 0;
		for (UINT32 lod = 0; lod < maxLod; ++lod)
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
			
			bool Simplified = false;
			for (const auto& group : groups)
			{
				std::vector<glm::ivec4> groupVertices;
		
				for (const auto& meshletIndex : group.meshlets)
				{
					const auto& meshlet = meshlets[meshletIndex];

					auto ptr = meshPack->m_MeshResource.vertices.attributes;
					groupVertices.insert(
						groupVertices.end(),
						ptr->begin() + 3 * meshlet.primitiveOffset, 
						ptr->begin() + 3 * meshlet.primitiveOffset + 3 * meshlet.nPrimitives
					);
				}
		
				/**
				* @brief Pack Sparse Inputs.
				*/
				std::vector<glm::vec3> packPoints;
				std::vector<glm::ivec3> packPrimPoints;
				std::vector<glm::ivec4> packVertices;
				std::unordered_map<uint32_t, uint32_t> primPointsMapReverse;
				PackVertexFromSparseInputs(meshPack, groupVertices, packPoints, packPrimPoints, packVertices, primPointsMapReverse);
		
				/**
				* @brief Simplify meshlets group primPoints.
				*/
				const float threshold   = 0.5f;
				size_t targetCount      = packPrimPoints.size() * threshold * 3;
				float targetError       = 0.1f * tLod + 0.01f * (1 - tLod);
				uint32_t options        = meshopt_SimplifyLockBorder;
			
				std::vector<glm::ivec3> simplifiedPrimPoints(packPrimPoints.size());
				float simplificationError = 0.0f;
		
				size_t simplifiedCount = meshopt_simplify(
					&simplifiedPrimPoints[0].x,
					&packPrimPoints[0].x,
					packPrimPoints.size() * 3,
					&packPoints[0].x,
					packPoints.size(),
					sizeof(glm::vec3),
					targetCount,
					targetError,
					options,
					&simplificationError
				);
				simplifiedPrimPoints.resize(simplifiedCount / 3);
		
				/**
				* @brief Simplify succeed: merge result to meshlets.
				*/
				if (simplifiedCount < packPrimPoints.size() * 3)
				{
					std::vector<glm::ivec3> primPointsBuffer;
					UnPackIndicesToSparseInputs(primPointsBuffer, primPointsMapReverse, simplifiedPrimPoints);
		
					AppendMeshlets(meshPack, lod + 1, primPointsBuffer);
		
					Simplified = true;
				}
		
				/**
				* @brief Simplify failed: try fuse points.
				*/
				else
				{
					scl::kd_tree<6> kdTree;
					BuildKDTree(packPoints, packPrimPoints, kdTree);
		
					float simplifyScale          = meshopt_simplifyScale(&packVertices[0].position.x, packVertices.size(), sizeof(Vertex));
					const float maxDistance      = (tLod * 0.1f + (1 - tLod) * 0.01f) * simplifyScale;
					const float maxUVDistance    = tLod * 0.5f + (1 - tLod) * 1.0f / 256.0f;
					auto vertexMap               = MergeByDistance(packVertices, kdTree, maxDistance, maxUVDistance);
		
					FindAndStableBoundaryVertices(packVertices, packIndices, vertexMap);
		
					/*for (int i = 0; i < packIndices.size(); i++)
					{
						uint32_t index = packIndices[i];
						packIndices[i] = vertexMap[index];
					}*/
		
					/**
					* @brief Simplify meshlets group indices again.
					*/
					size_t targetIndexCount = packIndices.size() * threshold;
		
					size_t simplifiedIndexCount = meshopt_simplify(
						simplifiedIndexBuffer.data(),
						packIndices.data(),
						packIndices.size(),
						&packVertices[0].position.x,
						packVertices.size(),
						sizeof(Vertex),
						targetIndexCount,
						targetError,
						options,
						&simplificationError
					);
					simplifiedIndexBuffer.resize(simplifiedIndexCount);
		
					if (simplifiedIndexCount < packIndices.size())
					{
						Simplified = true;
					}
		
					std::vector<uint32_t> indicesBuffer;
					UnPackIndicesToSparseInputs(indicesBuffer, indicesMapReverse, simplifiedIndexBuffer);
		
					AppendMeshlets(meshPack, lod + 1, indicesBuffer);
				}
		
			}
		
			auto out = std::chrono::high_resolution_clock::now();
			std::cout << "    Lod Cost: " << std::chrono::duration_cast<std::chrono::milliseconds>(out - in).count() << "    " << meshlets.size() << std::endl;
		
			/**
			* @brief return if cannot continue simplify meshlet.
			*/
			if (!Simplified) return;
		}
	}

	void MeshProcessor::AppendMeshlets(
		MeshPack*                      meshPack , 
		uint32_t                       lod      , 
		const std::vector<glm::ivec3>& primPoints
	)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief normal cone weight set 0.5f;
		* Get Const variable.
		*/
		const float coneWeight              = 0.5f;
		const uint32_t primLocationsOffset  = meshPack->m_MeshResource.primitiveLocations.attributes->size();
		const uint32_t primPointsOffset     = meshPack->m_MeshResource.primitivePoints.attributes->size();
		const uint32_t meshletsOffset       = meshPack->m_MeshResource.meshlets.attributes->size();
		
		/**
		* @brief Init meshopt variable.
		*/
		size_t max_meshlets = meshopt_buildMeshletsBound(primPoints.size() * 3, MESHLET_NVERTICES, MESHLET_NPRIMITIVES);
		std::vector<meshopt_Meshlet> meshoptlets(max_meshlets);
		std::vector<unsigned int> meshlet_vertices(max_meshlets * MESHLET_NVERTICES);
		std::vector<unsigned char> meshlet_triangles(max_meshlets * MESHLET_NPRIMITIVES * 3);
		
		/**
		* @brief Pack Sparse Inputs.
		*/
		std::vector<glm::vec3>  packPoints;
		std::vector<glm::ivec3> packPrimPoints;
		std::unordered_map<uint32_t, uint32_t> primPointsMapReverse;
		PackVertexFromSparseInputs(meshPack, primPoints, packPoints, packPrimPoints, primPointsMapReverse);
		
		/**
		* @brief Build Meshlets.
		*/
		size_t nMeshlet = meshopt_buildMeshlets(
			meshoptlets.data(), 
			meshlet_vertices.data(), 
			meshlet_triangles.data(), 
			&packPrimPoints[0].x,
			packPrimPoints.size() * 3,
			&packPoints[0].x,
			packPoints.size(),
			sizeof(glm::vec3), 
			MESHLET_NVERTICES, 
			MESHLET_NPRIMITIVES, 
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
					&meshlet_vertices[meshoptlets[i].vertex_offset],
					&meshlet_triangles[meshoptlets[i].triangle_offset],
					meshoptlets[i].triangle_count, meshoptlets[i].vertex_count
				);
		
				const meshopt_Meshlet& m = meshoptlets[i];
				meshopt_Bounds bounds = meshopt_computeMeshletBounds(
					&meshlet_vertices[m.vertex_offset],
					&meshlet_triangles[m.triangle_offset],
					m.triangle_count,
					&packPoints[0].x,
					packPoints.size(),
					sizeof(glm::vec3)
				);
		
				Meshlet meshlet;
				meshlet.FromMeshopt(meshoptlets[i], bounds);
				meshlet.primitiveOffset = nPrimitives;
		
				meshlet.vertexOffset    += primLocationsOffset;
				meshlet.primitiveOffset += primPointsOffset;
				meshlet.lod = lod;
		
				meshPack->m_MeshResource.meshlets.attributes->push_back(std::move(meshlet));
		
				nPrimitives += m.triangle_count;
			}
		}
		
		/**
		* @brief Fill in data back to meshpack variable.
		*/
		const Meshlet& lastm = (*meshPack->m_MeshResource.meshlets.attributes)[meshletsOffset + nMeshlet - 1];
		meshPack->m_MeshResource.primitivePoints    .attributes->resize( lastm.primitiveOffset + lastm.nPrimitives);
		meshPack->m_MeshResource.primitiveLocations .attributes->resize( lastm.primitiveOffset + lastm.nPrimitives);
		meshPack->m_MeshResource.vertices           .attributes->resize((lastm.primitiveOffset + lastm.nPrimitives) * 3);
		
		auto vertices = std::vector<glm::ivec4>(meshPack->m_MeshResource.vertices.attributes->begin() + 3 * primPointsOffset, meshPack->m_MeshResource.vertices.attributes->end());
		for (uint32_t i = 0; i < nMeshlet; i++)
		{
			const meshopt_Meshlet& m = meshoptlets[i];
			const Meshlet& ml = (*meshPack->m_MeshResource.meshlets.attributes)[meshletsOffset + i];
		
			for (uint32_t j = 0; j < m.triangle_count; j++)
			{
				uint32_t a = (uint32_t)meshlet_triangles[m.triangle_offset + 3 * j + 0] + m.vertex_offset;
				uint32_t b = (uint32_t)meshlet_triangles[m.triangle_offset + 3 * j + 1] + m.vertex_offset;
				uint32_t c = (uint32_t)meshlet_triangles[m.triangle_offset + 3 * j + 2] + m.vertex_offset;
		
				(*meshPack->m_MeshResource.primitivePoints.attributes)[ml.primitiveOffset + j].x = primPointsMapReverse[meshlet_vertices[a]];
				(*meshPack->m_MeshResource.primitivePoints.attributes)[ml.primitiveOffset + j].y = primPointsMapReverse[meshlet_vertices[b]];
				(*meshPack->m_MeshResource.primitivePoints.attributes)[ml.primitiveOffset + j].z = primPointsMapReverse[meshlet_vertices[c]];
		
				(*meshPack->m_MeshResource.primitiveLocations.attributes)[ml.primitiveOffset + j].x = a + primLocationsOffset;
				(*meshPack->m_MeshResource.primitiveLocations.attributes)[ml.primitiveOffset + j].y = b + primLocationsOffset;
				(*meshPack->m_MeshResource.primitiveLocations.attributes)[ml.primitiveOffset + j].z = c + primLocationsOffset;

				(*meshPack->m_MeshResource.vertices.attributes)[3 * ml.primitiveOffset + 3 * j + 0].x = primPointsMapReverse[meshlet_vertices[a]];
				(*meshPack->m_MeshResource.vertices.attributes)[3 * ml.primitiveOffset + 3 * j + 1].x = primPointsMapReverse[meshlet_vertices[b]];
				(*meshPack->m_MeshResource.vertices.attributes)[3 * ml.primitiveOffset + 3 * j + 2].x = primPointsMapReverse[meshlet_vertices[c]];
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
				const glm::ivec3& primPts = (*meshPack->m_MeshResource.primitivePoints.attributes)[meshlet.primitiveOffset + triangleIndex];

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

	std::vector<uint32_t> MeshProcessor::MergeByDistance(
		const std::vector<glm::vec3>& vertices      ,
		scl::kd_tree<6>&              kdTree        , 
		float                         maxDistance   , 
		float                         maxUVDistance
	)
	{
		SPICES_PROFILE_ZONE;

		std::vector<uint32_t> vertexRemap;
		vertexRemap.resize(vertices.size(), -1);

		/**
		* @brief fuse by distance.
		*/
		for (uint32_t v = 0; v < vertices.size(); v++)
		{
			//const Vertex& vertex = vertices[v];
			//auto items = kdTree.range_search(
			//	{ vertex.position.x, vertex.position.y, vertex.position.z, vertex.texCoord.x, vertex.texCoord.y, (float)v },
			//	{ maxDistance, maxDistance, maxDistance, maxUVDistance, maxUVDistance, (float)UINT32_MAX }
			//);
			//
			//for (const auto& item : items)
			//{
			//	if (vertexRemap[item[item.size() - 1]] == -1)
			//	{
			//		vertexRemap[item[item.size() - 1]] = v;
			//	}
			//}
		}

		/**
		* @brief Stable Points use original index.
		*/
		/*for (uint32_t v = 0; v < vertices.size(); v++)
		{
			if (boundary[v])
			{
				vertexRemap[v] = v;
			}
		}*/

		return vertexRemap;
	}

	bool MeshProcessor::BuildKDTree(const std::vector<glm::vec3>& points, scl::kd_tree<6>& kdTree)
	{
		SPICES_PROFILE_ZONE;

		std::vector<scl::kd_tree<6>::item> items;
		items.resize(points.size());

		for (int i = 0; i < points.size(); i++)
		{
			//const Vertex& vt = vertices[i];
			//points[i] = { vt.position.x, vt.position.y, vt.position.z, vt.texCoord.x, vt.texCoord.y, (float)i };
		}
		
		kdTree.insert(points);

		return true;
	}

	bool MeshProcessor::FindAndStableBoundaryVertices(
		const std::vector<glm::vec3>&    vertices    ,
		std::vector<uint32_t>&           indices     , 
		std::vector<uint32_t>&           verticesMap
	)
	{
		SPICES_PROFILE_ZONE;


		assert(indices.size() % 3 == 0);

		/**
		* @brief Get edge connected primitive.
		*/
		std::unordered_map<Edge, uint32_t> edgesConnects;

		for (uint32_t triangleIndex = 0; triangleIndex < indices.size() / 3; triangleIndex++)
		{
			for (uint32_t i = 0; i < 3; i++)
			{
				Edge edge;
				edge.first  = verticesMap[indices[  i           + triangleIndex * 3]];
				edge.second = verticesMap[indices[((i + 1) % 3) + triangleIndex * 3]];

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
		for (auto& pair : boundEdgePoints)
		{
			if (!pair.second.valid()) continue;

			//glm::vec3 l = glm::normalize(vertices[pair.second.next].position - vertices[pair.first].position);
			//glm::vec3 r = glm::normalize(vertices[pair.second.previous].position - vertices[pair.first].position);
			//
			//if (glm::dot(l, r) > -0.98f)
			//{
			//	//boundary[pair.first] = true;
			//}
		}

#endif

		return true;
	}

	bool MeshProcessor::PackVertexFromSparseInputs(
		MeshPack*                               meshPack          , 
		const std::vector<glm::ivec4>           vertices          ,
		std::vector<glm::vec3>&                 packPoints        ,
		std::vector<glm::ivec3>&                packPrimPoints    ,
		std::vector<glm::ivec4>&                packVertices      ,
		std::unordered_map<uint32_t, uint32_t>& primPointsMapReverse
	)
	{
		SPICES_PROFILE_ZONE;

		std::unordered_map<uint32_t, uint32_t> primPointsMap;  // key: original index, value: new index.

		for (const auto& vertex : vertices)
		{
			if (primPointsMap.find(vertex.x) == primPointsMap.end())
			{
				primPointsMap[vertex.x] = primPointsMap.size();
			}
		}

		packPoints.resize(primPointsMap.size());
		for (auto& pair : primPointsMap)
		{
			packPoints[pair.second] = (*meshPack->m_MeshResource.positions.attributes)[pair.first];
			primPointsMapReverse[pair.second] = pair.first;
		}

		packPrimPoints.resize(vertices.size() / 3);
		for (int i = 0; i < packPrimPoints.size(); i++)
		{
			packPrimPoints[i].x = primPointsMap[vertices[3 * i + 0].x];
			packPrimPoints[i].y = primPointsMap[vertices[3 * i + 1].x];
			packPrimPoints[i].z = primPointsMap[vertices[3 * i + 2].x];
		}

		packVertices = vertices;
		for (int i = 0; i < vertices.size(); i++)
		{

		}

		return true;
	}

	bool MeshProcessor::UnPackIndicesToSparseInputs(
		std::vector<glm::ivec3>&                primPoints           , 
		std::unordered_map<uint32_t, uint32_t>& primPointsMapReverse , 
		const std::vector<glm::ivec3>&          packPrimPoints
	)
	{
		SPICES_PROFILE_ZONE;

		primPoints.resize(packPrimPoints.size());
		for (int i = 0; i < packPrimPoints.size(); i++)
		{
			primPoints[i].x = primPointsMapReverse[packPrimPoints[i].x];
			primPoints[i].y = primPointsMapReverse[packPrimPoints[i].y];
			primPoints[i].z = primPointsMapReverse[packPrimPoints[i].z];
		}

		return true;
	}
}