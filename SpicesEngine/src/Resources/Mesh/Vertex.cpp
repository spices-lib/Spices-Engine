/**
* @file Vertex.cpp.
* @brief The Vertex Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "Vertex.h"

namespace Spices {

	std::vector<VkVertexInputBindingDescription> Vertex::GetBindingDescriptions()
	{
		SPICES_PROFILE_ZONE;

		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
		bindingDescriptions[0].binding    = 0;
		bindingDescriptions[0].stride     = sizeof(Vertex);
		bindingDescriptions[0].inputRate  = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescriptions;
	}

	std::vector<VkVertexInputAttributeDescription> Vertex::GetAttributeDescriptions()
	{
		SPICES_PROFILE_ZONE;

		std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

		attributeDescriptions.push_back({ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position) });
		attributeDescriptions.push_back({ 1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal)   });
		attributeDescriptions.push_back({ 2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)    });
		attributeDescriptions.push_back({ 3, 0, VK_FORMAT_R32G32_SFLOAT,    offsetof(Vertex, texCoord) });

		return attributeDescriptions;
	}

	void Meshlet::FromMeshopt(const meshopt_Meshlet& m, const meshopt_Bounds& bounds)
	{
		SPICES_PROFILE_ZONE;

		vertexOffset    = m.vertex_offset;
		nVertices       = m.vertex_count;
		primitiveOffset = 0;                   /* @brief Need more work here. */
		nPrimitives     = m.triangle_count;

		boundCenter.x   = bounds.center[0];
		boundCenter.y   = bounds.center[1];
		boundCenter.z   = bounds.center[2];

		boundRadius     = bounds.radius;

		coneApex.x      = bounds.cone_apex[0];
		coneApex.y      = bounds.cone_apex[1];
		coneApex.z      = bounds.cone_apex[2];
				        
		coneCutoff      = bounds.cone_cutoff;
				        
		coneAxis.x      = bounds.cone_axis[0];
		coneAxis.y      = bounds.cone_axis[1];
		coneAxis.z      = bounds.cone_axis[2];
	}
}