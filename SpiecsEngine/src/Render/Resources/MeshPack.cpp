#include "Pchheader.h"
#include "MeshPack.h"
#include "Render/Vulkan/VulkanRenderBackend.h"

namespace Spiecs {

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

	void SquarePack::OnCreatePack()
	{
		for (int i = 0; i < m_Rows; i++)
		{
			float rowRamp = i / float(m_Rows - 1) - 0.5f;  // -0.5f ~ 0.5f

			for (int j = 0; j < m_Colums; j++)
			{
				float colRamp = j / float(m_Colums - 1) - 0.5f; // -0.5f ~ 0.5f

				Vertex vt;
				vt.position = { colRamp, rowRamp, 0.0f};
				vt.normal = glm::normalize(vt.position);
				vt.color = glm::vec3{ 1.0f };
				vt.texCoord = { i / float(m_Rows), j / float(m_Colums) };

				m_Vertices.push_back(std::move(vt));
			}
		}

		for (int i = 0; i < m_Rows; i++)
		{
			for (int j = 0; j < m_Colums; j++)
			{
				if (i == (m_Rows - 1) || j == (m_Colums - 1)) continue;

				int vtIndex = i * m_Colums + j;

				m_Indices.push_back(vtIndex);
				m_Indices.push_back(vtIndex + 1);
				m_Indices.push_back(vtIndex + m_Colums + 1);

				m_Indices.push_back(vtIndex + m_Colums + 1);
				m_Indices.push_back(vtIndex + m_Colums);
				m_Indices.push_back(vtIndex);
			}
		}

		CreateBuffer();
	}

	void BoxPack::OnCreatePack()
	{
		m_Vertices =
		{
			// Front
			{ {-0.5f,  0.5f, -0.5f}, {0.0f,  0.0f, 1.0f}, {1.0f,  0.0f, 0.0f}, { 0.0f, 0.0f }},
			{ { 0.5f,  0.5f, -0.5f}, {0.0f,  0.0f, 1.0f}, {1.0f,  1.0f, 1.0f}, { 0.0f, 1.0f }},
			{ { 0.5f, -0.5f, -0.5f}, {0.0f,  0.0f, 1.0f}, {0.0f,  0.0f, 1.0f}, { 1.0f, 1.0f }},
			{ {-0.5f, -0.5f, -0.5f}, {0.0f,  0.0f, 1.0f}, {0.0f,  1.0f, 0.0f}, { 1.0f, 0.0f }},

			// Back
			{ { 0.5f,  0.5f,  0.5f}, {0.0f,  0.0f, 1.0f}, {1.0f,  0.0f, 0.0f}, { 0.0f, 0.0f }},
			{ {-0.5f,  0.5f,  0.5f}, {0.0f,  0.0f, 1.0f}, {0.0f,  1.0f, 0.0f}, { 1.0f, 0.0f }},
			{ {-0.5f, -0.5f,  0.5f}, {0.0f,  0.0f, 1.0f}, {0.0f,  0.0f, 1.0f}, { 1.0f, 1.0f }},
			{ { 0.5f, -0.5f,  0.5f}, {0.0f,  0.0f, 1.0f}, {1.0f,  1.0f, 1.0f}, { 0.0f, 1.0f }},

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
	
	void SpherePack::OnCreatePack()
	{
		for (int i = 0; i < m_Rows; i++)
		{
			float rowRamp = i / float(m_Rows - 1) * 180.0f; // 0 ~ 180

			for (int j = 0; j < m_Colums; j++)
			{
				float colRamp = j * 360.0 / float(m_Colums - 1); // 0 ~ 360

				Vertex vt;
				vt.position = { glm::sin(glm::radians(rowRamp)) * glm::sin(glm::radians(colRamp)), glm::cos(glm::radians(rowRamp)), glm::sin(glm::radians(rowRamp)) * glm::cos(glm::radians(colRamp)) };
				vt.normal = glm::normalize(vt.position);
				vt.color = glm::vec3{ 1.0f };
				vt.texCoord = { i / float(m_Rows), j / float(m_Colums) };

				m_Vertices.push_back(std::move(vt));
			}
		}

		for (int i = 0; i < m_Rows; i++)
		{
			for (int j = 0; j < m_Colums; j++)
			{
				if (i == (m_Rows - 1) || j == (m_Colums - 1)) continue;

				int vtIndex = i * m_Colums + j;

				m_Indices.push_back(vtIndex);
				m_Indices.push_back(vtIndex + 1);
				m_Indices.push_back(vtIndex + m_Colums + 1);

				m_Indices.push_back(vtIndex + m_Colums + 1);
				m_Indices.push_back(vtIndex + m_Colums);
				m_Indices.push_back(vtIndex);
			}
		}

		CreateBuffer();
	}

}