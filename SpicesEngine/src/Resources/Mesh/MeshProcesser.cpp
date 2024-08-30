/**
* @file MeshProcesser.cpp.
* @brief The MeshProcesser Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "MeshProcesser.h"
#include "MeshPack.h"

#include <src/meshoptimizer.h>
#include <metis.h>

namespace Spices {

	void MeshProcesser::GenerateMeshLodClusterHierarchy(MeshPack* meshPack)
	{
		SPICES_PROFILE_ZONE;

		size_t previousMeshletsStart = 0;
		AppendMeshlets(meshPack, meshPack->m_Indices);

		/*const int maxLod = 0;
		for (int lod = 0; lod < maxLod; ++lod)
		{
			float tLod = lod / (float)maxLod;

			std::vector<Meshlet> meshlets = std::vector<Meshlet>(meshPack->m_Meshlets.begin() + previousMeshletsStart, meshPack->m_Meshlets.end());
			if (meshlets.size() <= 1)
			{
				return;
			}

			std::vector<MeshletGroup> groups = GroupMeshlets(meshPack, meshlets);

			const size_t newMeshletStart = meshPack->m_Meshlets.size();
			for (const auto& group : groups)
			{
				std::vector<uint32_t> groupVertexIndices;

				for (const auto& meshletIndex : group.meshlets)
				{
					const auto& meshlet = meshlets[meshletIndex];
					size_t start = groupVertexIndices.size();
					groupVertexIndices.resize(start + meshlet.nPrimitives * 3);
					for (size_t j = 0; j < meshlet.nPrimitives * 3; j++)
					{
						groupVertexIndices[j + start] = meshPack->m_Indices[meshlet.primitiveOffset * 3 + j];
					}
				}

				const float threshold = 0.5f;
				size_t targetIndexCount = groupVertexIndices.size() * threshold;
				float targetError = 0.9f * tLod + 0.01f * (1 - tLod);
				uint32_t options = meshopt_SimplifyLockBorder;

				std::vector<uint32_t> simplifiedIndexBuffer;
				simplifiedIndexBuffer.resize(groupVertexIndices.size());
				float simplificationError = 0.0f;

				size_t simplifiedIndexCount = meshopt_simplify(simplifiedIndexBuffer.data(),
					groupVertexIndices.data(), groupVertexIndices.size(),
					&meshPack->m_Vertices[0].position.x, meshPack->m_Vertices.size(), sizeof(Vertex),
					targetIndexCount, targetError, options, &simplificationError
				);
				simplifiedIndexBuffer.resize(simplifiedIndexCount);

				AppendMeshlets(meshPack, simplifiedIndexBuffer);
				previousMeshletsStart = newMeshletStart;
			}
		}*/
	}

	void MeshProcesser::AppendMeshlets(MeshPack* meshPack, const std::vector<uint32_t> indices)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief normal cone weight set 0.5f;
		* Get Const variable.
		*/
		const float coneWeight              = 0.5f;
		const uint32_t vertexIndicesOffset  = meshPack->m_VertexIndices.size();
		const uint32_t indicesOffset        = meshPack->m_Indices.size();
		const uint32_t meshletsOffset       = meshPack->m_Meshlets.size();
		const uint32_t lod                  = meshletsOffset == 0 ? 0 : meshPack->m_Meshlets[meshletsOffset - 1].lod + 1;

		/** 
		* @brief Init meshopt variable.
		*/
		size_t max_meshlets = meshopt_buildMeshletsBound(indices.size(), MESHLET_NVERTICES, MESHLET_NPRIMITIVES);
		std::vector<meshopt_Meshlet> meshoptlets(max_meshlets);
		std::vector<unsigned int> meshlet_vertices(max_meshlets * MESHLET_NVERTICES);
		std::vector<unsigned char> meshlet_triangles(max_meshlets * MESHLET_NPRIMITIVES * 3);

		/**
		* @brief Build Meshlets.
		*/
		size_t nMeshlet = meshopt_buildMeshlets(meshoptlets.data(), meshlet_vertices.data(), meshlet_triangles.data(), indices.data(),
			indices.size(), &meshPack->m_Vertices[0].position.x, meshPack->m_Vertices.size(), sizeof(Vertex), MESHLET_NVERTICES, MESHLET_NPRIMITIVES, coneWeight);

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
		for (size_t i = 0; i < nMeshlet; ++i)
		{
			meshopt_optimizeMeshlet(&meshlet_vertices[meshoptlets[i].vertex_offset], &meshlet_triangles[meshoptlets[i].triangle_offset],
				meshoptlets[i].triangle_count, meshoptlets[i].vertex_count);

			const meshopt_Meshlet& m = meshoptlets[i];
			meshopt_Bounds bounds = meshopt_computeMeshletBounds(&meshlet_vertices[m.vertex_offset],
				&meshlet_triangles[m.triangle_offset], m.triangle_count, &meshPack->m_Vertices[0].position.x, meshPack->m_Vertices.size(), sizeof(Vertex));

			Meshlet meshlet;
			meshlet.FromMeshopt(meshoptlets[i], bounds);
			meshlet.primitiveOffset  = nPrimitives;

			meshlet.vertexOffset    += vertexIndicesOffset;
			meshlet.primitiveOffset += indicesOffset;
			meshlet.lod              = lod;

			meshPack->m_Meshlets.push_back(std::move(meshlet));

			nPrimitives += m.triangle_count;
		}

		/**
		* @brief Fill in data back to meshpack variable.
		*/
		const Meshlet& lastm = meshPack->m_Meshlets[meshletsOffset + nMeshlet - 1];
		meshPack->m_VertexIndices.resize(3 * (lastm.primitiveOffset + lastm.nPrimitives), 0);
		meshPack->m_Indices.resize(3 * (lastm.primitiveOffset + lastm.nPrimitives), 0);

		for (uint32_t i = 0; i < nMeshlet; i++)
		{
			const meshopt_Meshlet& m = meshoptlets[i];
			const Meshlet& ml = meshPack->m_Meshlets[meshletsOffset + i];

			for (uint32_t j = 0; j < m.triangle_count; j++)
			{
				uint32_t a = (uint32_t)meshlet_triangles[m.triangle_offset + 3 * j + 0] + m.vertex_offset;
				uint32_t b = (uint32_t)meshlet_triangles[m.triangle_offset + 3 * j + 1] + m.vertex_offset;
				uint32_t c = (uint32_t)meshlet_triangles[m.triangle_offset + 3 * j + 2] + m.vertex_offset;

				meshPack->m_Indices[3 * ml.primitiveOffset + 3 * j + 0] = meshlet_vertices[a];
				meshPack->m_Indices[3 * ml.primitiveOffset + 3 * j + 1] = meshlet_vertices[b];
				meshPack->m_Indices[3 * ml.primitiveOffset + 3 * j + 2] = meshlet_vertices[c];

				meshPack->m_VertexIndices[3 * ml.primitiveOffset + 3 * j + 0] = a + vertexIndicesOffset;
				meshPack->m_VertexIndices[3 * ml.primitiveOffset + 3 * j + 1] = b + vertexIndicesOffset;
				meshPack->m_VertexIndices[3 * ml.primitiveOffset + 3 * j + 2] = c + vertexIndicesOffset;
			}
		}
	}

	std::vector<MeshletGroup> MeshProcesser::GroupMeshlets(MeshPack* meshPack, const std::vector<Meshlet>& meshlets)
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

		std::unordered_map<Edge, std::vector<size_t>> edges2Meshlets;
		std::unordered_map<size_t, std::vector<Edge>> meshlets2Edges;

		for (size_t meshletIndex = 0; meshletIndex < meshlets.size(); meshletIndex++)
		{
			const auto& meshlet = meshlets[meshletIndex];
			auto getVertexIndex = [&](size_t index) 
			{
				return meshPack->m_VertexIndices[meshPack->m_Indices[index + meshlet.primitiveOffset * 3]];
			};

			const size_t triangleCount = meshlet.nPrimitives;

			for (size_t triangleIndex = 0; triangleIndex < triangleCount; triangleIndex++)
			{
				for (size_t i = 0; i < 3; i++)
				{
					Edge edge;
					edge.first  = getVertexIndex(i + triangleIndex * 3);
					edge.second = getVertexIndex(((i + 1) % 3) + triangleIndex * 3);

					edges2Meshlets[edge].push_back(meshletIndex);
					meshlets2Edges[meshletIndex].emplace_back(edge);
				}
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

		const auto& meshlet = meshlets[meshlets.size() - 1];
		idx_t vertexCount   = meshlet.vertexOffset + meshlet.nVertices;
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
							assert(d > 0 && d < edgeWeights.size());
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
}