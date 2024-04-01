#pragma once
#include "Core/Core.h"
#include "MeshPack.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Spiecs {

	// TODO: Move to ECS
	class TransformComponent
	{
	public:
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		glm::mat4 GetModelMatrix() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Position)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		};
	};

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