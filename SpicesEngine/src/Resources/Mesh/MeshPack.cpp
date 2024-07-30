/**
* @file MeshPack.cpp.
* @brief The MeshPack Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "MeshPack.h"
#include "Render/Vulkan/VulkanRenderBackend.h"
#include "Core/Library/ContainerLibrary.h"
#include "Resources/Loader/MeshLoader.h"

namespace Spices {
	
	void MeshPack::OnBind(VkCommandBuffer& commandBuffer) const
	{
		SPICES_PROFILE_ZONE;

		const VkBuffer buffers[] = { m_VertexBuffer->Get() };
		constexpr VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
		vkCmdBindIndexBuffer(commandBuffer, m_IndicesBuffer->Get(), 0, VK_INDEX_TYPE_UINT32);
	}

	void MeshPack::OnDraw(VkCommandBuffer& commandBuffer) const
	{
		SPICES_PROFILE_ZONE;

		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(m_Indices.size()), 1, 0, 0, 0);
	}

	void MeshPack::OnDrawMeshTasks(VkCommandBuffer& commandBuffer) const
	{
		SPICES_PROFILE_ZONE;

		static PFN_vkCmdDrawMeshTasksEXT vkCmdDrawMeshTasksEXT = reinterpret_cast<PFN_vkCmdDrawMeshTasksEXT>(vkGetInstanceProcAddr(VulkanRenderBackend::GetState().m_Instance, "vkCmdDrawMeshTasksEXT"));
		vkCmdDrawMeshTasksEXT(commandBuffer, m_Meshluts.size(), 1, 1);
	}

	void MeshPack::SetMaterial(const std::string& materialPath)
	{
		SPICES_PROFILE_ZONE;

		m_Material = ResourcePool<Material>::Load<Material>(materialPath);
		m_Material->BuildMaterial();
	}

	uint32_t MeshPack::GetHitShaderHandle() const
	{
		SPICES_PROFILE_ZONE;

		if(!m_HitShaderHandle.has_value())
		{
			std::stringstream ss;
			ss << "MeshPack do not has a vaild material handle.";
			
			SPICES_CORE_ERROR(ss.str());
		}
		
		return m_HitShaderHandle.value();
	}

	VulkanRayTracing::BlasInput MeshPack::MeshPackToVkGeometryKHR() const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief BLAS builder requires raw device addresses.
		*/
		const VkDeviceAddress vertexAddress          =  m_VertexBuffer->GetAddress();
		const VkDeviceAddress indicesAddress         =  m_IndicesBuffer->GetAddress();

		const uint32_t maxPrimitiveCount             = static_cast<uint32_t>(m_Indices.size() / 3);

		/**
		* @brief device pointer to the buffers holding triangle vertex/index data, 
		* along with information for interpreting it as an array (stride, data type, etc.)
		*/
		VkAccelerationStructureGeometryTrianglesDataKHR triangles {};
		triangles.sType                              = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
		triangles.vertexFormat                       = VK_FORMAT_R32G32B32_SFLOAT;  // vec3 vertex position data.
		triangles.vertexData.deviceAddress           = vertexAddress;          /* @note position needs to be the first attribute of Vertex, otherwise correct offset is needs to be added here. */
		triangles.vertexStride                       = sizeof(Vertex);
		triangles.indexType                          = VK_INDEX_TYPE_UINT32;
		triangles.indexData.deviceAddress            = indicesAddress;
	  //triangles.transformData = {};
		triangles.maxVertex                          = static_cast<uint32_t>(m_Vertices.size() - 1);

		/**
		* @brief wrapper around the above with the geometry type enum (triangles in this case) plus flags for the AS builder.
		*/
		VkAccelerationStructureGeometryKHR asGeom {};
		asGeom.sType                                 = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
		asGeom.geometryType                          = VK_GEOMETRY_TYPE_TRIANGLES_KHR;
		asGeom.flags                                 = VK_GEOMETRY_OPAQUE_BIT_KHR;
		asGeom.geometry.triangles                    = triangles;

		/**
		* @brief the indices within the vertex arrays to source input geometry for the BLAS.
		*/
		VkAccelerationStructureBuildRangeInfoKHR offset {};
		offset.firstVertex                           = 0;
		offset.primitiveCount                        = maxPrimitiveCount;
		offset.primitiveOffset                       = 0;
		offset.transformOffset                       = 0;

		/**
		* @brief Our blas is made from only one geometry, but could be made of many geometries.
		*/
		VulkanRayTracing::BlasInput input;
		input.asGeometry.emplace_back(asGeom);
		input.asBuildOffsetInfo.emplace_back(offset);

		return input;
	}

	void MeshPack::CreateBuffer()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Build meshlut buffer.
		*/
		{
			VkDeviceSize bufferSize = sizeof(SpicesShader::Meshlut) * m_Meshluts.size();

			VulkanBuffer stagingBuffer(
				VulkanRenderBackend::GetState(),
				bufferSize,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
				VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
			);

			void* data;
			vkMapMemory(VulkanRenderBackend::GetState().m_Device, stagingBuffer.GetMemory(), 0, bufferSize, 0, &data);
			memcpy(data, m_Meshluts.data(), (size_t)bufferSize);
			vkUnmapMemory(VulkanRenderBackend::GetState().m_Device, stagingBuffer.GetMemory());

			m_MeshlutsBuffer = std::make_shared<VulkanBuffer>(
				VulkanRenderBackend::GetState(),
				bufferSize,
				VK_BUFFER_USAGE_TRANSFER_DST_BIT                                     |
				VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT                            |
				VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR ,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			);

			m_MeshlutsBuffer->CopyBuffer(stagingBuffer.Get(), m_MeshlutsBuffer->Get(), bufferSize);
		}

		/*
		* @brief Build vertex buffer
		*/
		{
			VkDeviceSize bufferSize = sizeof(Vertex) * m_Vertices.size();

			VulkanBuffer stagingBuffer(
				VulkanRenderBackend::GetState(),
				bufferSize,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
				VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
			);

			void* data;
			vkMapMemory(VulkanRenderBackend::GetState().m_Device, stagingBuffer.GetMemory(), 0, bufferSize, 0, &data);
			memcpy(data, m_Vertices.data(), (size_t)bufferSize);
			vkUnmapMemory(VulkanRenderBackend::GetState().m_Device, stagingBuffer.GetMemory());

			m_VertexBuffer = std::make_shared<VulkanBuffer>(
				VulkanRenderBackend::GetState(),
				bufferSize, 
				VK_BUFFER_USAGE_TRANSFER_DST_BIT                                     | 
				VK_BUFFER_USAGE_VERTEX_BUFFER_BIT                                    |
				VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT                            |
				VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR ,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			);

			m_VertexBuffer->CopyBuffer(stagingBuffer.Get(), m_VertexBuffer->Get(), bufferSize);
		}

		/*
		* @brief Build index buffer.
		*/
		{
			VkDeviceSize bufferSize = sizeof(m_Indices[0]) * m_Indices.size();

			VulkanBuffer stagingBuffer(
				VulkanRenderBackend::GetState(),
				bufferSize,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
				VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
			);

			void* data;
			vkMapMemory(VulkanRenderBackend::GetState().m_Device, stagingBuffer.GetMemory(), 0, bufferSize, 0, &data);
			memcpy(data, m_Indices.data(), (size_t)bufferSize);
			vkUnmapMemory(VulkanRenderBackend::GetState().m_Device, stagingBuffer.GetMemory());

			m_IndicesBuffer = std::make_shared<VulkanBuffer>(
				VulkanRenderBackend::GetState(), 
				bufferSize, 
				VK_BUFFER_USAGE_TRANSFER_DST_BIT                                     | 
				VK_BUFFER_USAGE_INDEX_BUFFER_BIT                                     |
				VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT                            |
				VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR ,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			);

			m_IndicesBuffer->CopyBuffer(stagingBuffer.Get(), m_IndicesBuffer->Get(), bufferSize);
		}
	}

	void MeshPack::CreateMeshluts()
	{
		SPICES_PROFILE_ZONE;

		std::vector<Vertex> vertices = m_Vertices;
		std::vector<uint32_t> indices = m_Indices;

		std::list<uint32_t> fullIndices = std::list(indices.begin(), indices.end());

		m_Vertices.clear();
		m_Indices.clear();
		m_Meshluts.clear();

		int vertexIndex = 0;
		int primitiveIndex = 0;
		while (!fullIndices.empty())
		{
			SpicesShader::Meshlut        meshlut;
			meshlut.vertexOffset = vertexIndex;
			meshlut.primitiveOffset = primitiveIndex;

			scl::linked_unordered_map<uint32_t, uint32_t> localVertices;
			std::vector<uint32_t> localIndices;

			while (localVertices.size() < MESHLUTNVERTICES - 2 && localIndices.size() < MESHLUTNPRIMITIVES * 3 - 2 && !fullIndices.empty())
			{
				for (int j = 0; j < 3; j++)
				{
					uint32_t index = fullIndices.front();
					fullIndices.pop_front();
					if(!localVertices.has_key(index)) localVertices.push_back(index, localVertices.size());
					localIndices.push_back(index);
				}
			}

			assert(localVertices.size() <= MESHLUTNVERTICES);
			assert(localIndices.size() <= MESHLUTNPRIMITIVES * 3);

			meshlut.nVertices   = localVertices.size();
			meshlut.nPrimitives = localIndices.size() / 3;

			vertexIndex        += meshlut.nVertices;
			primitiveIndex     += meshlut.nPrimitives;

			m_Meshluts.push_back(std::move(meshlut));

			glm::vec3 center = glm::vec3(0.0f);
			localVertices.for_each([&](const uint32_t& k, const uint32_t& v) {
				m_Vertices.push_back(vertices[k]);
				center += vertices[k].position;
				return false;
			});
			center /= static_cast<float>(meshlut.nVertices);

			for (int i = 0; i < localIndices.size(); i++)
			{
				m_Indices.push_back(*localVertices.find_value(localIndices[i]) + meshlut.vertexOffset);
			}

			float radius = 0.0f;
			localVertices.for_each([&](const uint32_t& k, const uint32_t& v){
				float l = glm::distance(vertices[k].position, center);
				radius = glm::max(radius, l);
				return false;
			});

			meshlut.boundCenter = center;
			meshlut.boundRadius = radius;
		}
	}

	void MeshPack::ApplyMatrix(const glm::mat4& matrix)
	{
		SPICES_PROFILE_ZONE;

		for (uint64_t i = 0; i < m_Vertices.size(); i++)
		{
			glm::vec4 newPos = matrix * glm::vec4(m_Vertices[i].position, 1.0f);
			m_Vertices[i].position = { newPos.x, newPos.y, newPos.z };
		}
	}

	void MeshPack::CopyToVertices(std::vector<Vertex>& vertices) const
	{
		SPICES_PROFILE_ZONE;

		ContainerLibrary::Append<Vertex>(vertices, m_Vertices);
	}

	void MeshPack::CopyToIndices(std::vector<uint32_t>& indices, uint32_t offset)
	{
		SPICES_PROFILE_ZONE;

		for (uint64_t i = 0; i < m_Indices.size(); i++)
		{
			m_Indices[i] += offset;
		}

		ContainerLibrary::Append<uint32_t>(indices, m_Indices);
	}

	void SquarePack::OnCreatePack(bool isCreateBuffer)
	{
		SPICES_PROFILE_ZONE;

		for (uint32_t i = 0; i < m_Rows; i++)
		{
			float rowRamp = i / static_cast<float>(m_Rows - 1) - 0.5f;  // -0.5f ~ 0.5f

			for (uint32_t j = 0; j < m_Columns; j++)
			{
				float colRamp = j / static_cast<float>(m_Columns - 1) - 0.5f; // -0.5f ~ 0.5f

				Vertex vt;
				vt.position = { colRamp, rowRamp, 0.0f};
				vt.normal = glm::vec3(0.0f, 0.0f, 1.0f);
				vt.color = glm::vec3{ 1.0f };
				vt.texCoord = { colRamp + 0.5, 0.5 - rowRamp };

				m_Vertices.push_back(std::move(vt));
			}
		}

		for (uint32_t i = 0; i < m_Rows - 1; i++)
		{
			for (uint32_t j = 0; j < m_Columns - 1; j++)
			{
				const uint32_t vtIndex = i * m_Columns + j;

				m_Indices.push_back(vtIndex);
				m_Indices.push_back(vtIndex + 1);
				m_Indices.push_back(vtIndex + m_Columns + 1);

				m_Indices.push_back(vtIndex + m_Columns + 1);
				m_Indices.push_back(vtIndex + m_Columns);
				m_Indices.push_back(vtIndex);
			}
		}
		
		if (isCreateBuffer)
		{
			CreateMeshluts();
			CreateBuffer();
		}
	}

	void BoxPack::OnCreatePack(bool isCreateBuffer)
	{
		SPICES_PROFILE_ZONE;

		// Front
		{
			SquarePack pack(m_Rows, m_Columns);
			pack.OnCreatePack(false);
			glm::mat4 tran = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f));
			pack.ApplyMatrix(tran);
			pack.CopyToIndices(m_Indices, static_cast<uint32_t>(m_Indices.size()));
			pack.CopyToVertices(m_Vertices);
			
		}

		// Back
		{
			SquarePack pack(m_Rows, m_Columns);
			pack.OnCreatePack(false);
			glm::mat4 tran = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.5f));
			glm::mat4 rot = glm::toMat4(glm::quat({0.0f, glm::radians(180.0f), 0.0f}));
			pack.ApplyMatrix(tran * rot);
			pack.CopyToIndices(m_Indices, static_cast<uint32_t>(m_Vertices.size()));
			pack.CopyToVertices(m_Vertices);
			
		}

		// Left
		{
			SquarePack pack(m_Rows, m_Columns);
			pack.OnCreatePack(false);
			glm::mat4 tran = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));
			glm::mat4 rot = glm::toMat4(glm::quat({ 0.0f, glm::radians(-90.0f), 0.0f }));
			pack.ApplyMatrix(tran * rot);
			pack.CopyToIndices(m_Indices, static_cast<uint32_t>(m_Vertices.size()));
			pack.CopyToVertices(m_Vertices);
			
		}

		// Right
		{
			SquarePack pack(m_Rows, m_Columns);
			pack.OnCreatePack(false);
			glm::mat4 tran = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, 0.0f));
			glm::mat4 rot = glm::toMat4(glm::quat({ 0.0f, glm::radians(90.0f), 0.0f }));
			pack.ApplyMatrix(tran * rot);
			pack.CopyToIndices(m_Indices, static_cast<uint32_t>(m_Vertices.size()));
			pack.CopyToVertices(m_Vertices);
			
		}

		// Top
		{
			SquarePack pack(m_Rows, m_Columns);
			pack.OnCreatePack(false);
			glm::mat4 tran = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f));
			glm::mat4 rot = glm::toMat4(glm::quat({ glm::radians(-90.0f), 0.0f, 0.0f }));
			pack.ApplyMatrix(tran * rot);
			pack.CopyToIndices(m_Indices, static_cast<uint32_t>(m_Vertices.size()));
			pack.CopyToVertices(m_Vertices);
			
		}

		// Button
		{
			SquarePack pack(m_Rows, m_Columns);
			pack.OnCreatePack(false);
			glm::mat4 tran = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.0f));
			glm::mat4 rot = glm::toMat4(glm::quat({ glm::radians(90.0f), 0.0f, 0.0f }));
			pack.ApplyMatrix(tran * rot);
			pack.CopyToIndices(m_Indices, static_cast<uint32_t>(m_Vertices.size()));
			pack.CopyToVertices(m_Vertices);
			
		}

		if (isCreateBuffer)
		{
			CreateMeshluts();
			CreateBuffer();
		}
	}

	void FilePack::OnCreatePack(bool isCreateBuffer)
	{
		SPICES_PROFILE_ZONE;

		MeshLoader::Load(m_Path, this);
		if(isCreateBuffer) CreateBuffer();
	}
	
	void SpherePack::OnCreatePack(bool isCreateBuffer)
	{
		SPICES_PROFILE_ZONE;

		for (uint32_t i = 0; i < m_Rows; i++)
		{
			const float rowRamp = i / static_cast<float>(m_Rows - 1) * 180.0f; // 0 ~ 180

			for (uint32_t j = 0; j < m_Columns; j++)
			{
				const float colRamp = j * 360.0f / static_cast<float>(m_Columns - 1); // 0 ~ 360

				Vertex vt;
				vt.position = { glm::sin(glm::radians(rowRamp)) * glm::sin(glm::radians(colRamp)), glm::cos(glm::radians(rowRamp)), glm::sin(glm::radians(rowRamp)) * glm::cos(glm::radians(colRamp)) };
				vt.normal = glm::normalize(vt.position);
				vt.color = glm::vec3{ 1.0f };
				vt.texCoord = {j / static_cast<float>(m_Columns - 1), i / static_cast<float>(m_Rows - 1) };

				m_Vertices.push_back(std::move(vt));
			}
		}

		for (uint32_t i = 0; i < m_Rows - 1; i++)
		{
			for (uint32_t j = 0; j < m_Columns - 1; j++)
			{
				const uint32_t vtIndex = i * m_Columns + j;

				m_Indices.push_back(vtIndex);
				m_Indices.push_back(vtIndex + 1);
				m_Indices.push_back(vtIndex + m_Columns + 1);

				m_Indices.push_back(vtIndex + m_Columns + 1);
				m_Indices.push_back(vtIndex + m_Columns);
				m_Indices.push_back(vtIndex);
			}
		}

		if (isCreateBuffer)
		{
			CreateMeshluts();
			CreateBuffer();
		}
	}

}