#pragma once
#include "Core/Core.h"
#include "MeshPack.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Spiecs {

	class Mesh
	{
	public:
		class Builder
		{
		public:
			Builder() {};

			Builder& AddPack(std::shared_ptr<MeshPack> meshPack);
			std::shared_ptr<Mesh> Build() const;

		private:
			std::unordered_map<uint32_t, std::shared_ptr<MeshPack>> m_Pack;
			uint32_t m_PackNums;
		};

	public:
		Mesh(std::unordered_map<uint32_t, std::shared_ptr<MeshPack>> meshPacks);
		virtual ~Mesh() {};

		void Draw(VkCommandBuffer& commandBuffer);

	private:
		// Vertices Indices
		std::unordered_map<uint32_t, std::shared_ptr<MeshPack>> m_Pack;

		// Materials
		std::vector< std::shared_ptr<Material>> m_Materials;
	};
}