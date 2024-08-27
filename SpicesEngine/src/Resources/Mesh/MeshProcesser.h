#pragma once
#include "Core/Core.h"
#include "Vertex.h"

#include <metis.h>

namespace Spices {

	struct MeshletGroup 
	{
		std::vector<size_t> meshlets;
	};

	static std::vector<MeshletGroup> groupMeshlets(const std::vector<uint32_t>& indices,  const std::vector<Meshlet>& meshlets)
	{
		auto groupWithMeshlets = [&]() {
			MeshletGroup group;
			for (int i = 0; i < meshlets.size(); ++i)
			{
				group.meshlets.push_back(i);
			}
			return std::vector{ group };
		};


		if (meshlets.size() < 8)
		{
			return groupWithMeshlets();
		}

		std::unordered_map<Edge, std::vector<size_t>> edges2Meshlets;
		std::unordered_map<size_t, std::vector<Edge>> meshlets2Edges;

		for (size_t meshletIndex = 0; meshletIndex < meshlets.size(); meshletIndex++)
		{
			const auto& meshlet = meshlets[meshletIndex];
			auto getVertexIndex = [&](size_t index) {
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

		// remove internal edge.
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

		if (edges2Meshlets.empty())
		{
			return groupWithMeshlets();
		}

		idx_t vertexCount = meshlets.size();
		idx_t ncon = 1;
		idx_t nparts = meshlets.size() / 4;
		assert(nparts > 1);
		idx_t options[METIS_NOPTIONS];
		METIS_SetDefaultOptions(options);


		options[METIS_OPTION_OBJTYPE] = METIS_OBJTYPE_CUT;
		options[METIS_OPTION_CCORDER] = 1;

		std::vector<idx_t> partition;
		partition.resize(vertexCount);

		std::vector<idx_t> xadjacency;
		xadjacency.reserve(vertexCount + 1);

		std::vector<idx_t> edgeAdjacency;
		std::vector<idx_t> edgeWeights;

		for (size_t meshletIndex = 0; meshletIndex < meshlets.size(); meshletIndex++)
		{
			size_t startIndexInEdgeAdjacency = edgeAdjacency.size();
			for (const auto& edge : meshlets2Edges[meshletIndex])
			{
				auto connectionsIter = edges2Meshlets.find(edge);
				if (connectionsIter == edges2Meshlets.end())
				{
					continue;
				}
				const auto& connections = connectionsIter->second;
				for (const auto& connectedMeshlet : connections)
				{
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
							assert(d > 0);
							assert(d < edgeWeights.size());
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