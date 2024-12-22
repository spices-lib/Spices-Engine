/**
* @file Vertex.cpp.
* @brief The Vertex Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "Vertex.h"

namespace Spices {
	
	std::vector<VkVertexInputBindingDescription> InputAssembly::GetBindingDescriptions()
	{
		SPICES_PROFILE_ZONE;

		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);

		bindingDescriptions[0].binding   = 0;
		bindingDescriptions[0].stride    = sizeof(glm::vec3);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return std::move(bindingDescriptions);
	}

	std::vector<VkVertexInputAttributeDescription> InputAssembly::GetAttributeDescriptions()
	{
		SPICES_PROFILE_ZONE;

		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(1);

		attributeDescriptions[0].binding  = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format   = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset   = 0;

		return std::move(attributeDescriptions);
	}

	std::vector<VkVertexInputBindingDescription> InputAssembly::GetSlateBindingDescriptions()
	{
		SPICES_PROFILE_ZONE;

		std::vector<VkVertexInputBindingDescription> bindingDescriptions(3);

		bindingDescriptions[0].binding   = 0;
		bindingDescriptions[0].stride    = sizeof(glm::vec2);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		bindingDescriptions[1].binding   = 1;
		bindingDescriptions[1].stride    = sizeof(glm::vec2);
		bindingDescriptions[1].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		bindingDescriptions[2].binding   = 2;
		bindingDescriptions[2].stride    = sizeof(float);
		bindingDescriptions[2].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return std::move(bindingDescriptions);
	}

	std::vector<VkVertexInputAttributeDescription> InputAssembly::GetSlateAttributeDescriptions()
	{
		SPICES_PROFILE_ZONE;

		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(3);

		attributeDescriptions[0].binding  = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format   = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset   = 0;

		attributeDescriptions[1].binding  = 1;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format   = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[1].offset   = 0;

		attributeDescriptions[2].binding  = 2;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format   = VK_FORMAT_R8G8B8A8_UNORM;
		attributeDescriptions[2].offset   = 0;

		return std::move(attributeDescriptions);
	}


	void Meshlet::FromMeshopt(const meshopt_Meshlet& m, const meshopt_Bounds& bounds)
	{
		SPICES_PROFILE_ZONE;

		vertexOffset           = m.vertex_offset;
		primitiveOffset        = 0;                   /* @brief Need more work here. */
		nVertices              = m.vertex_count;
		nPrimitives            = m.triangle_count;
							   
		lod                    = 0;
						       
		boundSphere.c.x        = bounds.center[0];
		boundSphere.c.y        = bounds.center[1];
		boundSphere.c.z        = bounds.center[2];
		boundSphere.r          = bounds.radius;
							   
		clusterBoundSphere.c   = glm::vec3(0.0f);
		clusterBoundSphere.r   = 0.0f;
							   
		coneApex.x             = bounds.cone_apex[0];
		coneApex.y             = bounds.cone_apex[1];
		coneApex.z             = bounds.cone_apex[2];
						       
		coneAxis.x             = bounds.cone_axis[0];
		coneAxis.y             = bounds.cone_axis[1];
		coneAxis.z             = bounds.cone_axis[2];
						       
		coneCutoff             = bounds.cone_cutoff;
	}
}