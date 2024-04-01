#include "pchheader.h"
#include "MeshPack.h"

namespace Spiecs {

	void SquarePack::OnCreatePack()
	{
		m_Vertices =
		{
			{ {-1.0f,  1.0f, 0.0f}, {0.0f,  0.0f, 1.0f}, {1.0f,  0.0f, 0.0f}, { 0.0f, 0.0f }},
			{ { 1.0f,  1.0f, 0.0f}, {0.0f,  0.0f, 1.0f}, {0.0f,  1.0f, 0.0f}, { 1.0f, 0.0f }},
			{ { 1.0f, -1.0f, 0.0f}, {0.0f,  0.0f, 1.0f}, {0.0f,  0.0f, 1.0f}, { 1.0f, 1.0f }},
			{ {-1.0f, -1.0f, 0.0f}, {0.0f,  0.0f, 1.0f}, {1.0f,  1.0f, 1.0f}, { 0.0f, 1.0f }}
		};

		m_Indices = { 0, 1, 2, 2, 3, 0 };

		CreateBuffer();
	}

	void FilePack::OnCreatePack()
	{
		// TODO:
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

			VulkanBuffer stagingBuffer(m_VulkanState, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			void* data;
			vkMapMemory(m_VulkanState.m_Device, stagingBuffer.GetMomory(), 0, bufferSize, 0, &data);
			memcpy(data, m_Vertices.data(), (size_t)bufferSize);
			vkUnmapMemory(m_VulkanState.m_Device, stagingBuffer.GetMomory());

			m_VertexBuffer = std::make_unique<VulkanBuffer>(m_VulkanState, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

			m_VertexBuffer->CopyBuffer(stagingBuffer.Get(), m_VertexBuffer->Get(), bufferSize);
		}

		// index buffer
		{
			VkDeviceSize bufferSize = sizeof(m_Indices[0]) * m_Indices.size();

			VulkanBuffer stagingBuffer(m_VulkanState, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			void* data;
			vkMapMemory(m_VulkanState.m_Device, stagingBuffer.GetMomory(), 0, bufferSize, 0, &data);
			memcpy(data, m_Indices.data(), (size_t)bufferSize);
			vkUnmapMemory(m_VulkanState.m_Device, stagingBuffer.GetMomory());

			m_IndicesBuffer = std::make_unique<VulkanBuffer>(m_VulkanState, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

			m_IndicesBuffer->CopyBuffer(stagingBuffer.Get(), m_IndicesBuffer->Get(), bufferSize);
		}
	}
}