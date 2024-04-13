#include "Pchheader.h"
#include "MeshPack.h"
#include "Render/Vulkan/VulkanRenderBackend.h"

namespace Spiecs {

	void SquarePack::OnCreatePack()
	{
		m_Vertices =
		{
			{ {-1.0f,  1.0f, 0.0f}, {0.0f,  0.0f, 1.0f}, {1.0f,  0.0f, 0.0f}, { 0.0f, 0.0f }},
			{ { 1.0f,  1.0f, 0.0f}, {0.0f,  0.0f, 1.0f}, {0.0f,  1.0f, 0.0f}, { 1.0f, 0.0f }},
			{ { 1.0f, -1.0f, 0.0f}, {0.0f,  0.0f, 1.0f}, {0.0f,  0.0f, 1.0f}, { 1.0f, 1.0f }},
			{ {-1.0f, -1.0f, 0.0f}, {0.0f,  0.0f, 1.0f}, {1.0f,  1.0f, 1.0f}, { 0.0f, 1.0f }},
		};

		m_Indices = { 0, 1, 2, 2, 3, 0 };

		CreateBuffer();
	}

	void BoxPack::OnCreatePack()
	{
		m_Vertices =
		{
			// Front
			{ {-0.5f,  0.5f,  0.5f}, {0.0f,  0.0f, 1.0f}, {1.0f,  0.0f, 0.0f}, { 0.0f, 0.0f }},
			{ { 0.5f,  0.5f,  0.5f}, {0.0f,  0.0f, 1.0f}, {0.0f,  1.0f, 0.0f}, { 1.0f, 0.0f }},
			{ { 0.5f, -0.5f,  0.5f}, {0.0f,  0.0f, 1.0f}, {0.0f,  0.0f, 1.0f}, { 1.0f, 1.0f }},
			{ {-0.5f, -0.5f,  0.5f}, {0.0f,  0.0f, 1.0f}, {1.0f,  1.0f, 1.0f}, { 0.0f, 1.0f }},

			// Back
			{ {-0.5f,  0.5f, -0.5f}, {0.0f,  0.0f, 1.0f}, {1.0f,  0.0f, 0.0f}, { 0.0f, 0.0f }},
			{ {-0.5f, -0.5f, -0.5f}, {0.0f,  0.0f, 1.0f}, {1.0f,  1.0f, 1.0f}, { 0.0f, 1.0f }},
			{ { 0.5f, -0.5f, -0.5f}, {0.0f,  0.0f, 1.0f}, {0.0f,  0.0f, 1.0f}, { 1.0f, 1.0f }},
			{ { 0.5f,  0.5f, -0.5f}, {0.0f,  0.0f, 1.0f}, {0.0f,  1.0f, 0.0f}, { 1.0f, 0.0f }},

			// Left
			{ {-0.5f,  0.5f,  0.5f}, {0.0f,  0.0f, 1.0f}, {1.0f,  0.0f, 0.0f}, { 0.0f, 0.0f }},
			{ {-0.5f,  0.5f, -0.5f}, {0.0f,  0.0f, 1.0f}, {1.0f,  1.0f, 1.0f}, { 0.0f, 1.0f }},
			{ {-0.5f, -0.5f, -0.5f}, {0.0f,  0.0f, 1.0f}, {0.0f,  0.0f, 1.0f}, { 1.0f, 1.0f }},
			{ {-0.5f, -0.5f,  0.5f}, {0.0f,  0.0f, 1.0f}, {0.0f,  1.0f, 0.0f}, { 1.0f, 0.0f }},

			// Right
			{ { 0.5f,  0.5f, -0.5f}, {0.0f,  0.0f, 1.0f}, {1.0f,  0.0f, 0.0f}, { 0.0f, 0.0f }},
			{ { 0.5f,  0.5f,  0.5f}, {0.0f,  0.0f, 1.0f}, {1.0f,  1.0f, 1.0f}, { 0.0f, 1.0f }},
			{ { 0.5f, -0.5f,  0.5f}, {0.0f,  0.0f, 1.0f}, {0.0f,  0.0f, 1.0f}, { 1.0f, 1.0f }},
			{ { 0.5f, -0.5f, -0.5f}, {0.0f,  0.0f, 1.0f}, {0.0f,  1.0f, 0.0f}, { 1.0f, 0.0f }},

			// Top
			{ {-0.5f, -0.5f, -0.5f}, {0.0f,  0.0f, 1.0f}, {1.0f,  0.0f, 0.0f}, { 0.0f, 0.0f }},
			{ { 0.5f, -0.5f, -0.5f}, {0.0f,  0.0f, 1.0f}, {1.0f,  1.0f, 1.0f}, { 0.0f, 1.0f }},
			{ { 0.5f, -0.5f,  0.5f}, {0.0f,  0.0f, 1.0f}, {0.0f,  0.0f, 1.0f}, { 1.0f, 1.0f }},
			{ {-0.5f, -0.5f,  0.5f}, {0.0f,  0.0f, 1.0f}, {0.0f,  1.0f, 0.0f}, { 1.0f, 0.0f }},

			// Bottum
			{ { 0.5f,  0.5f, -0.5f}, {0.0f,  0.0f, 1.0f}, {1.0f,  0.0f, 0.0f}, { 0.0f, 0.0f }},
			{ {-0.5f,  0.5f, -0.5f}, {0.0f,  0.0f, 1.0f}, {1.0f,  1.0f, 1.0f}, { 0.0f, 1.0f }},
			{ {-0.5f,  0.5f,  0.5f}, {0.0f,  0.0f, 1.0f}, {0.0f,  0.0f, 1.0f}, { 1.0f, 1.0f }},
			{ { 0.5f,  0.5f,  0.5f}, {0.0f,  0.0f, 1.0f}, {0.0f,  1.0f, 0.0f}, { 1.0f, 0.0f }},
		};

		m_Indices = 
		{ 
			 0,  1,  2,  2,  3,  0, 
			 4,  5,  6,  6,  7,  4,
			 8,  9, 10, 10, 11,  8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 21, 22, 22, 23, 20,

		};

		CreateBuffer();
	}

	void FilePack::OnCreatePack()
	{
		MeshLoader::Load(m_Path, this);
		CreateBuffer();
	}

	void MeshPack::SetMaterial(std::shared_ptr<Material> material)
	{
		m_Material = material;
		m_Material->BuildMaterial();
	}

	void MeshPack::OnBind(VkCommandBuffer& commandBuffer)
	{
		VkBuffer buffers[] = { m_VertexBuffer->Get() };
		VkDeviceSize offests[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offests);
		vkCmdBindIndexBuffer(commandBuffer, m_IndicesBuffer->Get(), 0, VK_INDEX_TYPE_UINT32);
	}

	void MeshPack::OnDraw(VkCommandBuffer& commandBuffer)
	{
		vkCmdDrawIndexed(commandBuffer, m_Indices.size(), 1, 0, 0, 0);
	}

	void MeshPack::CreateBuffer()
	{
		// vertex buffer
		{
			VkDeviceSize bufferSize = sizeof(Vertex) * m_Vertices.size();

			VulkanBuffer stagingBuffer(VulkanRenderBackend::GetState(), bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			void* data;
			vkMapMemory(VulkanRenderBackend::GetState().m_Device, stagingBuffer.GetMomory(), 0, bufferSize, 0, &data);
			memcpy(data, m_Vertices.data(), (size_t)bufferSize);
			vkUnmapMemory(VulkanRenderBackend::GetState().m_Device, stagingBuffer.GetMomory());

			m_VertexBuffer = std::make_unique<VulkanBuffer>(VulkanRenderBackend::GetState(), bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

			m_VertexBuffer->CopyBuffer(stagingBuffer.Get(), m_VertexBuffer->Get(), bufferSize);
		}

		// index buffer
		{
			VkDeviceSize bufferSize = sizeof(m_Indices[0]) * m_Indices.size();

			VulkanBuffer stagingBuffer(VulkanRenderBackend::GetState(), bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			void* data;
			vkMapMemory(VulkanRenderBackend::GetState().m_Device, stagingBuffer.GetMomory(), 0, bufferSize, 0, &data);
			memcpy(data, m_Indices.data(), (size_t)bufferSize);
			vkUnmapMemory(VulkanRenderBackend::GetState().m_Device, stagingBuffer.GetMomory());

			m_IndicesBuffer = std::make_unique<VulkanBuffer>(VulkanRenderBackend::GetState(), bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

			m_IndicesBuffer->CopyBuffer(stagingBuffer.Get(), m_IndicesBuffer->Get(), bufferSize);
		}
	}

	
}