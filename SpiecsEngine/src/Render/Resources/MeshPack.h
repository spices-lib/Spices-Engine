#pragma once
#include "Core/Core.h"
#include "Vertex.h"
#include "Render/Vulkan/VulkanBuffer.h"
#include "Material.h"

namespace Spiecs {

	class MeshPack
	{
	public:
		MeshPack(VulkanState& vulkanState) : m_VulkanState(vulkanState) {};
		virtual ~MeshPack() {};

		MeshPack(const MeshPack&) = delete;
		MeshPack& operator=(const MeshPack&) = delete;

		virtual void OnCreatePack() {};

		void OnBind(VkCommandBuffer& commandBuffer);
		void OnDraw(VkCommandBuffer& commandBuffer);

		const std::vector<Vertex>& GetVertices() { return m_Vertices; };
		const std::vector<uint32_t>& GetIndices() { return m_Indices; };

	protected:
		void CreateBuffer();

	protected:
		VulkanState& m_VulkanState;

		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;

		std::unique_ptr<VulkanBuffer> m_VertexBuffer;
		std::unique_ptr<VulkanBuffer> m_IndicesBuffer;

		std::shared_ptr<Material> m_Material;
	};

	class SquarePack : public MeshPack
	{
	public:
		SquarePack(VulkanState& vulkanState) : MeshPack(vulkanState) {};

		virtual void OnCreatePack() override;
	};

	class FilePack : public MeshPack
	{
	public:
		FilePack(VulkanState& vulkanState, const std::string& filePath) : MeshPack(vulkanState), m_Path(filePath) {};

		virtual void OnCreatePack() override;
	private:
		std::string m_Path;
	};
}