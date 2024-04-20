/**
* @file MeshPack.cpp.
* @brief The MeshPack Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "MeshPack.h"
#include "Render/Vulkan/VulkanRenderBackend.h"
#include "Core/Library/ContainerLibrary.h"
#include "Resources/Loader/MeshLoader.h"

namespace Spiecs {

	void MeshPack::OnBind(VkCommandBuffer& commandBuffer)
	{
		VkBuffer buffers[] = { m_VertexBuffer->Get() };
		VkDeviceSize offests[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offests);
		vkCmdBindIndexBuffer(commandBuffer, m_IndicesBuffer->Get(), 0, VK_INDEX_TYPE_UINT32);
	}

	void MeshPack::OnDraw(VkCommandBuffer& commandBuffer)
	{
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(m_Indices.size()), 1, 0, 0, 0);
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

	void MeshPack::ApplyMatrix(const glm::mat4& matrix)
	{
		for (int i = 0; i < m_Vertices.size(); i++)
		{
			glm::vec4 newPos = matrix * glm::vec4(m_Vertices[i].position, 1.0f);
			m_Vertices[i].position = { newPos.x, newPos.y, newPos.z };
		}
	}

	void MeshPack::CopyToVertices(std::vector<Vertex>& vertices)
	{
		ContainerLibrary::Append<Vertex>(vertices, m_Vertices);
	}

	void MeshPack::CopyToIndices(std::vector<uint32_t>& indices, uint32_t offest)
	{
		for (int i = 0; i < m_Indices.size(); i++)
		{
			m_Indices[i] += offest;
		}

		ContainerLibrary::Append<uint32_t>(indices, m_Indices);
	}

	void SquarePack::OnCreatePack(bool isCreateBuffer)
	{
		for (uint32_t i = 0; i < m_Rows; i++)
		{
			float rowRamp = i / float(m_Rows - 1) - 0.5f;  // -0.5f ~ 0.5f

			for (uint32_t j = 0; j < m_Colums; j++)
			{
				float colRamp = j / float(m_Colums - 1) - 0.5f; // -0.5f ~ 0.5f

				Vertex vt;
				vt.position = { colRamp, rowRamp, 0.0f};
				vt.normal = glm::normalize(vt.position);
				vt.color = glm::vec3{ 1.0f };
				vt.texCoord = { colRamp + 0.5, 0.5 - rowRamp };

				m_Vertices.push_back(std::move(vt));
			}
		}

		for (uint32_t i = 0; i < m_Rows; i++)
		{
			for (uint32_t j = 0; j < m_Colums; j++)
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

		if (isCreateBuffer) CreateBuffer();
	}

	void BoxPack::OnCreatePack(bool isCreateBuffer)
	{	
		// Front
		{
			SquarePack pack(m_Rows, m_Colums);
			pack.OnCreatePack(false);
			glm::mat4 tran = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f));
			pack.ApplyMatrix(tran);
			pack.CopyToIndices(m_Indices, static_cast<uint32_t>(m_Indices.size()));
			pack.CopyToVertices(m_Vertices);
			
		}

		// Back
		{
			SquarePack pack(m_Rows, m_Colums);
			pack.OnCreatePack(false);
			glm::mat4 tran = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.5f));
			glm::mat4 rot = glm::toMat4(glm::quat({0.0f, glm::radians(180.0f), 0.0f}));
			pack.ApplyMatrix(tran * rot);
			pack.CopyToIndices(m_Indices, static_cast<uint32_t>(m_Vertices.size()));
			pack.CopyToVertices(m_Vertices);
			
		}

		// Left
		{
			SquarePack pack(m_Rows, m_Colums);
			pack.OnCreatePack(false);
			glm::mat4 tran = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));
			glm::mat4 rot = glm::toMat4(glm::quat({ 0.0f, glm::radians(-90.0f), 0.0f }));
			pack.ApplyMatrix(tran * rot);
			pack.CopyToIndices(m_Indices, static_cast<uint32_t>(m_Vertices.size()));
			pack.CopyToVertices(m_Vertices);
			
		}

		// Right
		{
			SquarePack pack(m_Rows, m_Colums);
			pack.OnCreatePack(false);
			glm::mat4 tran = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, 0.0f));
			glm::mat4 rot = glm::toMat4(glm::quat({ 0.0f, glm::radians(90.0f), 0.0f }));
			pack.ApplyMatrix(tran * rot);
			pack.CopyToIndices(m_Indices, static_cast<uint32_t>(m_Vertices.size()));
			pack.CopyToVertices(m_Vertices);
			
		}

		// Top
		{
			SquarePack pack(m_Rows, m_Colums);
			pack.OnCreatePack(false);
			glm::mat4 tran = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f));
			glm::mat4 rot = glm::toMat4(glm::quat({ glm::radians(-90.0f), 0.0f, 0.0f }));
			pack.ApplyMatrix(tran * rot);
			pack.CopyToIndices(m_Indices, static_cast<uint32_t>(m_Vertices.size()));
			pack.CopyToVertices(m_Vertices);
			
		}

		// Button
		{
			SquarePack pack(m_Rows, m_Colums);
			pack.OnCreatePack(false);
			glm::mat4 tran = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.0f));
			glm::mat4 rot = glm::toMat4(glm::quat({ glm::radians(90.0f), 0.0f, 0.0f }));
			pack.ApplyMatrix(tran * rot);
			pack.CopyToIndices(m_Indices, static_cast<uint32_t>(m_Vertices.size()));
			pack.CopyToVertices(m_Vertices);
			
		}

		if (isCreateBuffer) CreateBuffer();
	}

	void FilePack::OnCreatePack(bool isCreateBuffer)
	{
		MeshLoader::Load(m_Path, this);
		if(isCreateBuffer) CreateBuffer();
	}
	
	void SpherePack::OnCreatePack(bool isCreateBuffer)
	{
		for (uint32_t i = 0; i < m_Rows; i++)
		{
			float rowRamp = i / float(m_Rows - 1) * 180.0f; // 0 ~ 180

			for (uint32_t j = 0; j < m_Colums; j++)
			{
				float colRamp = j * 360.0f / float(m_Colums - 1); // 0 ~ 360

				Vertex vt;
				vt.position = { glm::sin(glm::radians(rowRamp)) * glm::sin(glm::radians(colRamp)), glm::cos(glm::radians(rowRamp)), glm::sin(glm::radians(rowRamp)) * glm::cos(glm::radians(colRamp)) };
				vt.normal = glm::normalize(vt.position);
				vt.color = glm::vec3{ 1.0f };
				vt.texCoord = {j / float(m_Colums - 1), i / float(m_Rows - 1) };

				m_Vertices.push_back(std::move(vt));
			}
		}

		for (uint32_t i = 0; i < m_Rows; i++)
		{
			for (uint32_t j = 0; j < m_Colums; j++)
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

		if (isCreateBuffer) CreateBuffer();
	}

}