#pragma once
#include "Core/Core.h"
#include <vector>
#include <iostream>

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include <glm/gtx/hash.hpp>

namespace Spiecs {

	class Vertex 
	{
	public:
		glm::vec3 position{};
		glm::vec3 normal{};
		glm::vec3 color{};
		glm::vec2 texCoord{};

		static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
		static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();

		bool operator==(const Vertex& other) const {
			return position == other.position && color == other.color && normal == other.normal && texCoord == other.texCoord;
		};
	};
}

namespace std {

	template<> struct hash<Spiecs::Vertex> {
		size_t operator()(Spiecs::Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.position) ^
					(hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^
					(hash<glm::vec3>()(vertex.color) << 1) >> 1 ^
					(hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}