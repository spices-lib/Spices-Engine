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

#include <src/meshoptimizer.h>

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
		vkCmdDrawMeshTasksEXT(commandBuffer, static_cast<uint32_t>(m_Meshlets.size()), 1, 1);
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
		* @brief Build meshlet buffer.
		*/
		{
			VkDeviceSize bufferSize = sizeof(SpicesShader::Meshlet) * m_Meshlets.size();

			VulkanBuffer stagingBuffer(
				VulkanRenderBackend::GetState(),
				bufferSize,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
				VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
			);

			void* data;
			vkMapMemory(VulkanRenderBackend::GetState().m_Device, stagingBuffer.GetMemory(), 0, bufferSize, 0, &data);
			memcpy(data, m_Meshlets.data(), (size_t)bufferSize);
			vkUnmapMemory(VulkanRenderBackend::GetState().m_Device, stagingBuffer.GetMemory());

			m_MeshletsBuffer = std::make_shared<VulkanBuffer>(
				VulkanRenderBackend::GetState(),
				bufferSize,
				VK_BUFFER_USAGE_TRANSFER_DST_BIT                                     |
				VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT                            |
				VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR ,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			);

			m_MeshletsBuffer->CopyBuffer(stagingBuffer.Get(), m_MeshletsBuffer->Get(), bufferSize);
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

	void MeshPack::CreateMeshlets()
	{
		SPICES_PROFILE_ZONE;

		const float coneWeight = 0.5f;

		size_t max_meshlets = meshopt_buildMeshletsBound(m_Indices.size(), MESHLET_NVERTICES, MESHLET_NPRIMITIVES);
		std::vector<meshopt_Meshlet> meshlets(max_meshlets);
		std::vector<unsigned int> meshlet_vertices(max_meshlets * MESHLET_NVERTICES);
		std::vector<unsigned char> meshlet_triangles(max_meshlets * MESHLET_NPRIMITIVES * 3);

		size_t nMeshlet = meshopt_buildMeshlets(meshlets.data(), meshlet_vertices.data(), meshlet_triangles.data(), m_Indices.data(),
			m_Indices.size(), &m_Vertices[0].position.x, m_Vertices.size(), sizeof(Vertex), MESHLET_NVERTICES, MESHLET_NPRIMITIVES, coneWeight);

		const meshopt_Meshlet& last = meshlets[nMeshlet - 1];
		meshlets.resize(nMeshlet);
		meshlet_vertices.resize(last.vertex_offset + last.vertex_count);
		meshlet_triangles.resize(last.triangle_offset + (last.triangle_count * 3 + 3) & ~3);

		uint32_t nPrimitives = 0;
		for (size_t i = 0; i < nMeshlet; ++i)
		{
			meshopt_optimizeMeshlet(&meshlet_vertices[meshlets[i].vertex_offset], &meshlet_triangles[meshlets[i].triangle_offset], 
				meshlets[i].triangle_count, meshlets[i].vertex_count);

			const meshopt_Meshlet& m = meshlets[i];
			meshopt_Bounds bounds = meshopt_computeMeshletBounds(&meshlet_vertices[m.vertex_offset],
				&meshlet_triangles[m.triangle_offset], m.triangle_count, &m_Vertices[0].position.x, m_Vertices.size(), sizeof(Vertex));

			SpicesShader::Meshlet       meshlet;
			meshlet.vertexOffset      = meshlets[i].vertex_offset;
			meshlet.nVertices         = meshlets[i].vertex_count;
			meshlet.primitiveOffset   = nPrimitives;
			meshlet.nPrimitives       = meshlets[i].triangle_count;

			meshlet.boundCenter.x     = bounds.center[0];
			meshlet.boundCenter.y     = bounds.center[1];
			meshlet.boundCenter.z     = bounds.center[2];

			meshlet.boundRadius       = bounds.radius;

			m_Meshlets.push_back(std::move(meshlet));

			nPrimitives += m.triangle_count;
		}

		std::vector<Vertex> tempVertices = m_Vertices;
		std::vector<uint32_t> tempIndices = m_Indices;

		m_Vertices.clear();
		m_Indices.clear();

		m_Vertices.resize(meshlet_vertices.size());

		const SpicesShader::Meshlet& lastm = m_Meshlets[m_Meshlets.size() - 1];
		m_Indices.resize(3 * (lastm.primitiveOffset + lastm.nPrimitives), 0);

		for (uint32_t i = 0; i < meshlet_vertices.size(); i++)
		{
			m_Vertices[i] = std::move(tempVertices[meshlet_vertices[i]]);
		}

		for (uint32_t i = 0; i < nMeshlet; i++)
		{
			const meshopt_Meshlet& m = meshlets[i];
			const SpicesShader::Meshlet& ml = m_Meshlets[i];

			for (uint32_t j = 0; j < m.triangle_count; j++)
			{
				m_Indices[3 * ml.primitiveOffset + 3 * j + 0] = (uint32_t)meshlet_triangles[m.triangle_offset + 3 * j + 0] + m.vertex_offset;
				m_Indices[3 * ml.primitiveOffset + 3 * j + 1] = (uint32_t)meshlet_triangles[m.triangle_offset + 3 * j + 1] + m.vertex_offset;
				m_Indices[3 * ml.primitiveOffset + 3 * j + 2] = (uint32_t)meshlet_triangles[m.triangle_offset + 3 * j + 2] + m.vertex_offset;
			}
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
			CreateMeshlets();
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
			CreateMeshlets();
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
			CreateMeshlets();
			CreateBuffer();
		}
	}

}