/**
* @file MeshPack.cpp.
* @brief The MeshPack Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "MeshPack.h"
#include "Render/Vulkan/VulkanRenderBackend.h"
#include "Resources/Loader/MeshLoader.h"

namespace Spices {
	
	MeshDesc::MeshDesc()
	{
		SPICES_PROFILE_ZONE;

		modelAddress             = 0;
		vertexAddress            = 0;
		indexAddress             = 0;
		materialParameterAddress = 0;
		meshletAddress           = 0;
		nMeshlets                = 0;
		entityID                 = 0;

		m_Buffer = std::make_shared<VulkanBuffer>(
			VulkanRenderBackend::GetState(),
			sizeof(SpicesShader::MeshDesc),
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT         |
			VK_BUFFER_USAGE_TRANSFER_DST_BIT         |
			VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT      |
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		);

		m_Buffer->WriteToBuffer(this);
	}

	MeshDesc MeshDesc::Copy()
	{
		SPICES_PROFILE_ZONE;

		MeshDesc desc;
		desc.m_Buffer->CopyBuffer(m_Buffer->Get(), desc.m_Buffer->Get(), sizeof(SpicesShader::MeshDesc));

		return desc;
	}

	MeshPack::MeshPack(const std::string& name, bool instanced)
		: m_UUID(UUID())
		, m_MeshPackName(name)
		, m_Instanced(instanced)
	{}

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

		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(m_NIndices), 1, 0, 0, 0);
	}

	void MeshPack::OnDrawMeshTasks(VkCommandBuffer& commandBuffer) const
	{
		SPICES_PROFILE_ZONE;

		VulkanRenderBackend::GetState().m_VkFunc.vkCmdDrawMeshTasksEXT(commandBuffer, m_NTasks, 1, 1);
	}

	bool MeshPack::OnCreatePack(bool isCreateBuffer)
	{
		SPICES_PROFILE_ZONE;
		
		auto ptr = ResourcePool<MeshPack>::Load(m_MeshPackName);

		if (m_Instanced || !ptr) return false;

		/**
		* @brief Copy Data from ResourcePool.
		*/
		m_Desc                              = ptr->m_Desc.Copy();
		m_VertexBuffer                      = ptr->m_VertexBuffer;
		m_IndicesBuffer                     = ptr->m_IndicesBuffer;
		m_MeshletsBuffer                    = ptr->m_MeshletsBuffer;
		m_MeshTaskIndirectDrawCommand       = ptr->m_MeshTaskIndirectDrawCommand;
		m_NIndices                          = ptr->m_NIndices;
		m_NVertices                         = ptr->m_NVertices;
		m_NMeshlets                         = ptr->m_NMeshlets;

		if (m_Material)
		{
			m_Desc.UpdatematerialParameterAddress(m_Material->GetMaterialParamsAddress());
		}

		return true;
	}

	void MeshPack::SetMaterial(const std::string& materialPath)
	{
		SPICES_PROFILE_ZONE;

		m_Material = ResourcePool<Material>::Load<Material>(materialPath, materialPath);
		m_Material->BuildMaterial();

		m_Desc.UpdatematerialParameterAddress(m_Material->GetMaterialParamsAddress());
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

	uint32_t MeshPack::GetShaderGroupHandle() const
	{
		SPICES_PROFILE_ZONE;

		if (!m_ShaderGroupHandle.has_value())
		{
			std::stringstream ss;
			ss << "MeshPack do not has a vaild material handle.";

			SPICES_CORE_ERROR(ss.str());
		}

		return m_ShaderGroupHandle.value();
	}

	VulkanRayTracing::BlasInput MeshPack::MeshPackToVkGeometryKHR() const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief BLAS builder requires raw device addresses.
		*/
		const VkDeviceAddress vertexAddress          =  m_VertexBuffer->GetAddress();
		const VkDeviceAddress indicesAddress         =  m_IndicesBuffer->GetAddress();

		const uint32_t maxPrimitiveCount             = static_cast<uint32_t>(m_NIndices / 3);

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
		triangles.maxVertex                          = static_cast<uint32_t>(m_NVertices - 1);

		/**
		* @brief wrapper around the above with the geometry type enum (triangles in this case) plus flags for the AS builder.
		*/
		VkAccelerationStructureGeometryKHR             asGeom {};
		asGeom.sType                                 = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
		asGeom.geometryType                          = VK_GEOMETRY_TYPE_TRIANGLES_KHR;
		asGeom.flags                                 = VK_GEOMETRY_OPAQUE_BIT_KHR;
		asGeom.geometry.triangles                    = triangles;

		/**
		* @brief the indices within the vertex arrays to source input geometry for the BLAS.
		*/
		VkAccelerationStructureBuildRangeInfoKHR       offset {};
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
			m_NMeshlets = m_Meshlets.size();

			VkDeviceSize bufferSize = sizeof(SpicesShader::Meshlet) * m_NMeshlets;

			VulkanBuffer stagingBuffer(
				VulkanRenderBackend::GetState(),
				bufferSize,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
				VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
			);

			stagingBuffer.WriteToBuffer(m_Meshlets.data());

			m_MeshletsBuffer = std::make_shared<VulkanBuffer>(
				VulkanRenderBackend::GetState(),
				bufferSize,
				VK_BUFFER_USAGE_TRANSFER_DST_BIT                                     |
				VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT                            |
				VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR ,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			);

			m_MeshletsBuffer->CopyBuffer(stagingBuffer.Get(), m_MeshletsBuffer->Get(), bufferSize);

			m_NTasks = m_NMeshlets / SUBGROUP_SIZE + 1;

			m_MeshTaskIndirectDrawCommand.firstTask = 0;
			m_MeshTaskIndirectDrawCommand.taskCount = m_NTasks;

			m_Desc.UpdatenMeshlets(m_NMeshlets);
			m_Desc.UpdatemeshletAddress(m_MeshletsBuffer->GetAddress());
		}

		/*
		* @brief Build vertex buffer
		*/
		{
			m_NVertices = m_Vertices.size();
			
			VkDeviceSize bufferSize = sizeof(Vertex) * m_NVertices;

			VulkanBuffer stagingBuffer(
				VulkanRenderBackend::GetState(),
				bufferSize,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
				VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
			);

			stagingBuffer.WriteToBuffer(m_Vertices.data());

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

			m_Desc.UpdatevertexAddress(m_VertexBuffer->GetAddress());
		}

		/*
		* @brief Build index buffer.
		*/
		{
			m_NIndices = m_Indices.size();

			VkDeviceSize bufferSize = sizeof(m_Indices[0]) * m_NIndices;

			VulkanBuffer stagingBuffer(
				VulkanRenderBackend::GetState(),
				bufferSize,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
				VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
			);

			stagingBuffer.WriteToBuffer(m_Indices.data());

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

			m_Desc.UpdateindexAddress(m_IndicesBuffer->GetAddress());
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

		vertices.insert(vertices.end(), m_Vertices.begin(), m_Vertices.end());
	}

	void MeshPack::CopyToIndices(std::vector<uint32_t>& indices, uint32_t offset)
	{
		SPICES_PROFILE_ZONE;

		for (uint64_t i = 0; i < m_Indices.size(); i++)
		{
			m_Indices[i] += offset;
		}

		indices.insert(indices.end(), m_Indices.begin(), m_Indices.end());
	}

	bool PlanePack::OnCreatePack(bool isCreateBuffer)
	{
		SPICES_PROFILE_ZONE;

		if (MeshPack::OnCreatePack(isCreateBuffer)) return true;

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

				m_Vertices.push_back(vt);
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

		return true;
	}

	bool CubePack::OnCreatePack(bool isCreateBuffer)
	{
		SPICES_PROFILE_ZONE;

		if (MeshPack::OnCreatePack(isCreateBuffer)) return true;

		// Front
		{
			PlanePack pack(m_Rows, m_Columns);
			pack.OnCreatePack(false);
			glm::mat4 tran = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f));
			pack.ApplyMatrix(tran);
			pack.CopyToIndices(m_Indices, static_cast<uint32_t>(m_Indices.size()));
			pack.CopyToVertices(m_Vertices);
			
		}

		// Back
		{
			PlanePack pack(m_Rows, m_Columns);
			pack.OnCreatePack(false);
			glm::mat4 tran = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.5f));
			glm::mat4 rot = glm::toMat4(glm::quat({0.0f, glm::radians(180.0f), 0.0f}));
			pack.ApplyMatrix(tran * rot);
			pack.CopyToIndices(m_Indices, static_cast<uint32_t>(m_Vertices.size()));
			pack.CopyToVertices(m_Vertices);
			
		}

		// Left
		{
			PlanePack pack(m_Rows, m_Columns);
			pack.OnCreatePack(false);
			glm::mat4 tran = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));
			glm::mat4 rot = glm::toMat4(glm::quat({ 0.0f, glm::radians(-90.0f), 0.0f }));
			pack.ApplyMatrix(tran * rot);
			pack.CopyToIndices(m_Indices, static_cast<uint32_t>(m_Vertices.size()));
			pack.CopyToVertices(m_Vertices);
			
		}

		// Right
		{
			PlanePack pack(m_Rows, m_Columns);
			pack.OnCreatePack(false);
			glm::mat4 tran = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, 0.0f));
			glm::mat4 rot = glm::toMat4(glm::quat({ 0.0f, glm::radians(90.0f), 0.0f }));
			pack.ApplyMatrix(tran * rot);
			pack.CopyToIndices(m_Indices, static_cast<uint32_t>(m_Vertices.size()));
			pack.CopyToVertices(m_Vertices);
			
		}

		// Top
		{
			PlanePack pack(m_Rows, m_Columns);
			pack.OnCreatePack(false);
			glm::mat4 tran = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f));
			glm::mat4 rot = glm::toMat4(glm::quat({ glm::radians(-90.0f), 0.0f, 0.0f }));
			pack.ApplyMatrix(tran * rot);
			pack.CopyToIndices(m_Indices, static_cast<uint32_t>(m_Vertices.size()));
			pack.CopyToVertices(m_Vertices);
			
		}

		// Button
		{
			PlanePack pack(m_Rows, m_Columns);
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

		return true;
	}

	bool FilePack::OnCreatePack(bool isCreateBuffer)
	{
		SPICES_PROFILE_ZONE;

		if (MeshPack::OnCreatePack(isCreateBuffer)) return true;

		MeshLoader::Load(m_Path, this);
		if(isCreateBuffer) CreateBuffer();

		return true;
	}
	
	bool SpherePack::OnCreatePack(bool isCreateBuffer)
	{
		SPICES_PROFILE_ZONE;

		if (MeshPack::OnCreatePack(isCreateBuffer)) return true;

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

		return true;
	}

}