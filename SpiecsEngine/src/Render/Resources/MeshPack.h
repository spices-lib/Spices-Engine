#pragma once
#include "Core/Core.h"
#include "Vertex.h"
#include "Render/Material/Material.h"
#include "Render/Vulkan/VulkanBuffer.h"
#include "Render/Resources/Loader/MeshLoader.h"

namespace Spiecs {

	class MeshPack
	{
	public:
		MeshPack();
		virtual ~MeshPack() {};

		MeshPack(const MeshPack&) = delete;
		MeshPack& operator=(const MeshPack&) = delete;

		virtual void OnCreatePack() {};
		void SetMaterial(std::shared_ptr<Material> material) { m_Material = material; };
		inline std::shared_ptr<Material> GetMaterial() { return m_Material; };

		void OnBind(VkCommandBuffer& commandBuffer);
		void OnDraw(VkCommandBuffer& commandBuffer);

		const std::vector<Vertex>& GetVertices() { return m_Vertices; };
		const std::vector<uint32_t>& GetIndices() { return m_Indices; };

	protected:
		void CreateBuffer();

	protected:

		std::vector<Vertex> m_Vertices{};
		std::vector<uint32_t> m_Indices{};

		std::unique_ptr<VulkanBuffer> m_VertexBuffer;
		std::unique_ptr<VulkanBuffer> m_IndicesBuffer;

		std::shared_ptr<Material> m_Material;

		friend class MeshLoader;
	};

	class SquarePack : public MeshPack
	{
	public:
		SquarePack() : MeshPack() {};

		virtual void OnCreatePack() override;
	};

	class FilePack : public MeshPack
	{
	public:
		FilePack(const std::string& filePath) : MeshPack(), m_Path(filePath) {};

		virtual void OnCreatePack() override;
	private:
		std::string m_Path;
	};
}