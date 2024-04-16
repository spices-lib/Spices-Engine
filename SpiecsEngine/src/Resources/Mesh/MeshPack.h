#pragma once
#include "Core/Core.h"
#include "Vertex.h"
#include "Resources/Material/Material.h"
#include "Render/Vulkan/VulkanBuffer.h"
#include "Resources/ResourcePool/ResourcePool.h"
#include "Resources/Material/MeshMaterial.h"

namespace Spiecs {

	class MeshPack
	{
	public:
		MeshPack() {};
		virtual ~MeshPack() {};

		MeshPack(const MeshPack&) = delete;
		MeshPack& operator=(const MeshPack&) = delete;

		virtual void OnCreatePack(bool isCreateBuffer = true) {};

		template<typename T>
		void SetMaterial(const std::string& materialPath);

		inline std::shared_ptr<Material> GetMaterial() { return m_Material; };

		void OnBind(VkCommandBuffer& commandBuffer);
		void OnDraw(VkCommandBuffer& commandBuffer);

		const std::vector<Vertex>& GetVertices() { return m_Vertices; };
		const std::vector<uint32_t>& GetIndices() { return m_Indices; };

	protected:
		void CreateBuffer();

	public:
		/**
		* @brief Transform Vertices Position before CreateBuffer
		*/
		void ApplyMatrix(glm::mat4 matrix);

		/**
		* @brief Copy this Vertices to another pack
		*/
		void CopyToVertices(std::vector<Vertex>& vertices);

		/**
		* @brief Copy this Indices to another pack
		*/
		void CopyToIndices(std::vector<uint32_t>& indices, uint32_t offest = 0);

	protected:

		std::vector<Vertex> m_Vertices{};
		std::vector<uint32_t> m_Indices{};

		std::unique_ptr<VulkanBuffer> m_VertexBuffer;
		std::unique_ptr<VulkanBuffer> m_IndicesBuffer;

		std::shared_ptr<Material> m_Material;

		friend class MeshLoader;
	};

	template<typename T>
	inline void MeshPack::SetMaterial(const std::string& materialPath)
	{
		m_Material = ResourcePool<Material>::Load<T>(materialPath);
		m_Material->BuildMaterial();
	}

	class SquarePack : public MeshPack
	{
	public:
		SquarePack(uint32_t rows = 2, uint32_t colums = 2)
			: MeshPack(), m_Rows(rows), m_Colums(colums) {};

		virtual void OnCreatePack(bool isCreateBuffer = true) override;

	private:
		uint32_t m_Rows;
		uint32_t m_Colums;
	};

	class BoxPack : public MeshPack
	{
	public:
		BoxPack(uint32_t rows = 2, uint32_t colums = 2)
			: MeshPack(), m_Rows(rows), m_Colums(colums) {};

		virtual void OnCreatePack(bool isCreateBuffer = true) override;

	private:
		uint32_t m_Rows;
		uint32_t m_Colums;
	};

	class SpherePack : public MeshPack
	{
	public:
		SpherePack(uint32_t rows = 15, uint32_t colums = 24)
			: MeshPack(), m_Rows(rows), m_Colums(colums) {};

		virtual void OnCreatePack(bool isCreateBuffer = true) override;

	private:
		uint32_t m_Rows;
		uint32_t m_Colums;
	};

	class FilePack : public MeshPack
	{
	public:
		FilePack(const std::string& filePath) : MeshPack(), m_Path(filePath) {};

		virtual void OnCreatePack(bool isCreateBuffer = true) override;
	private:
		std::string m_Path;
	};

	
}