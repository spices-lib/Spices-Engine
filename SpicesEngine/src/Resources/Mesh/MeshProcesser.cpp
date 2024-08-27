/**
* @file MeshProcesser.cpp.
* @brief The MeshProcesser Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "MeshProcesser.h"

#include <src/meshoptimizer.h>
#include <metis.h>

namespace Spices {

	void MeshProcesser::CreateMeshlets(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, std::vector<Meshlet>& meshlets)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief normal cone weight set 0.5f;
		*/
		const float coneWeight = 0.5f;

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
			indices.size(), &vertices[0].position.x, vertices.size(), sizeof(Vertex), MESHLET_NVERTICES, MESHLET_NPRIMITIVES, coneWeight);

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
				&meshlet_triangles[m.triangle_offset], m.triangle_count, &vertices[0].position.x, vertices.size(), sizeof(Vertex));

			Meshlet meshlet;
			meshlet.FromMeshopt(meshoptlets[i], bounds);
			meshlet.primitiveOffset = nPrimitives;

			meshlets.push_back(std::move(meshlet));

			nPrimitives += m.triangle_count;
		}

		/**
		* @brief Fill in data back to meshpack variable.
		*/
		std::vector<Vertex> tempVertices = vertices;
		std::vector<uint32_t> tempIndices = indices;

		vertices.clear();
		indices.clear();

		vertices.resize(meshlet_vertices.size());

		const Meshlet& lastm = meshlets[meshlets.size() - 1];
		indices.resize(3 * (lastm.primitiveOffset + lastm.nPrimitives), 0);

		for (uint32_t i = 0; i < meshlet_vertices.size(); i++)
		{
			vertices[i] = std::move(tempVertices[meshlet_vertices[i]]);
		}

		for (uint32_t i = 0; i < nMeshlet; i++)
		{
			const meshopt_Meshlet& m = meshoptlets[i];
			const SpicesShader::Meshlet& ml = meshlets[i];

			for (uint32_t j = 0; j < m.triangle_count; j++)
			{
				indices[3 * ml.primitiveOffset + 3 * j + 0] = (uint32_t)meshlet_triangles[m.triangle_offset + 3 * j + 0] + m.vertex_offset;
				indices[3 * ml.primitiveOffset + 3 * j + 1] = (uint32_t)meshlet_triangles[m.triangle_offset + 3 * j + 1] + m.vertex_offset;
				indices[3 * ml.primitiveOffset + 3 * j + 2] = (uint32_t)meshlet_triangles[m.triangle_offset + 3 * j + 2] + m.vertex_offset;
			}
		}
	}

	std::vector<MeshletGroup> MeshProcesser::GroupMeshlets(const std::vector<uint32_t>& indices, const std::vector<Meshlet>& meshlets)
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
				return index + meshlet.primitiveOffset * 3;
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

	/*{
		for (const auto& group : groups)
		{
			std::vector<uint32_t> groupVertexIndices;
			for (const auto& meshletIndex : group.meshlets)
			{
				const auto& meshlet = ;

				size_t start = groupVertexIndices.size();
				groupVertexIndices.resize(start + meshlet.indexCount);
				for (size_t j = 0; j < meshlet.indexCount; j++)
				{
					groupVertexIndices[j + start] = ;
				}
			}
		}
	}*/
}