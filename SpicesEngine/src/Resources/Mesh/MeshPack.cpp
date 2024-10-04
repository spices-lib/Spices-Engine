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
	
	std::vector<VkVertexInputBindingDescription> MeshResource::GetBindingDescriptions()
	{
		SPICES_PROFILE_ZONE;

		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(glm::vec3);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescriptions;
	}

	std::vector<VkVertexInputAttributeDescription> MeshResource::GetAttributeDescriptions()
	{
		SPICES_PROFILE_ZONE;

		std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

		attributeDescriptions.push_back({ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0 });

		return attributeDescriptions;
	}

	void MeshResource::CreateBuffer(const std::string& name)
	{
		SPICES_PROFILE_ZONE;

		positions          .CreateBuffer(name + "PositionsBuffer", VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR);
		normals            .CreateBuffer(name + "NormalsBuffer"            );
		colors             .CreateBuffer(name + "ColorsBuffer"             );
		texCoords          .CreateBuffer(name + "TexCoordsBuffer"          );
		vertices           .CreateBuffer(name + "VerticesBuffer"           );
		primitivePoints    .CreateBuffer(name + "PrimitivePointsBuffer", VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR);
		primitiveVertices  .CreateBuffer(name + "PrimitiveVerticesBuffer"  );
		primitiveLocations .CreateBuffer(name + "PrimitiveLocationsBuffer" );
		meshlets           .CreateBuffer(name + "MeshletsBuffer"           );
	}

	MeshDesc::MeshDesc()
	{
		SPICES_PROFILE_ZONE;

		modelAddress                = 0;
		positionsAddress            = 0;
		normalsAddress              = 0;
		colorsAddress               = 0;
		texCoordsAddress            = 0;
		verticesAddress             = 0;
		primitivePointsAddress      = 0;
		primitiveVerticesAddress    = 0;
		primitiveLocationsAddress   = 0;
		materialParameterAddress    = 0;
		meshletsAddress             = 0;
		nMeshlets                   = 0;
		entityID                    = 0;

		m_Buffer = std::make_shared<VulkanBuffer>(
			VulkanRenderBackend::GetState()          ,
			"MeshDescBuffer"                         ,
			sizeof(SpicesShader::MeshDesc)           ,
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
		, m_NTasks(0)
	{}

	void MeshPack::OnBind(VkCommandBuffer& commandBuffer) const
	{
		SPICES_PROFILE_ZONE;

		const VkBuffer buffers[] = { m_MeshResource.positions.buffer->Get() };
		constexpr VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
		vkCmdBindIndexBuffer(commandBuffer, m_MeshResource.primitivePoints.buffer->Get(), 0, VK_INDEX_TYPE_UINT32);
	}

	void MeshPack::OnDraw(VkCommandBuffer& commandBuffer) const
	{
		SPICES_PROFILE_ZONE;

		int lodLevel = 0;
		auto ptr = m_Material->GetConstantParams().find_value("lod");
		if (ptr)
		{
			lodLevel = std::any_cast<int>(ptr->value.paramValue);
			lodLevel = std::max(lodLevel, 0);
			lodLevel = std::min(lodLevel, (int)m_MeshResource.lods.attributes->size() - 1);
		}
		const Lod& lod0 = (*m_MeshResource.lods.attributes)[lodLevel];
		vkCmdDrawIndexed(commandBuffer, lod0.nPrimitives * 3, 1, lod0.primVertexOffset * 3, 0, 0);
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
		m_MeshResource                      = ptr->m_MeshResource;

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
		const VkDeviceAddress vertexAddress          = m_MeshResource.positions.buffer->GetAddress();
		const VkDeviceAddress indicesAddress         = m_MeshResource.primitivePoints.buffer->GetAddress();

		const Lod& lod0                              = (*m_MeshResource.lods.attributes)[0];
		const uint32_t maxPrimitiveCount             = lod0.nPrimitives;

		/**
		* @brief device pointer to the buffers holding triangle vertex/index data, 
		* along with information for interpreting it as an array (stride, data type, etc.)
		*/
		VkAccelerationStructureGeometryTrianglesDataKHR triangles {};
		triangles.sType                              = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
		triangles.vertexFormat                       = VK_FORMAT_R32G32B32_SFLOAT;  // vec3 vertex position data.
		triangles.vertexData.deviceAddress           = vertexAddress;               /* @note position needs to be the first attribute of Vertex, otherwise correct offset is needs to be added here. */
		triangles.vertexStride                       = sizeof(glm::vec3);
		triangles.indexType                          = VK_INDEX_TYPE_UINT32;
		triangles.indexData.deviceAddress            = indicesAddress;
	  //triangles.transformData = {};
		triangles.maxVertex                          = static_cast<uint32_t>(m_MeshResource.primitivePoints.attributes->size() * 3 - 1);

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
		offset.primitiveOffset                       = lod0.primVertexOffset;
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

		m_NTasks = m_MeshResource.meshlets.attributes->size() / SUBGROUP_SIZE + 1;
		m_MeshTaskIndirectDrawCommand.firstTask = 0;
		m_MeshTaskIndirectDrawCommand.taskCount = m_NTasks;

		m_MeshResource.CreateBuffer(m_MeshPackName);

		m_Desc.UpdatepositionsAddress          (m_MeshResource.positions.buffer                  );
		m_Desc.UpdatenormalsAddress            (m_MeshResource.normals.buffer                    );
		m_Desc.UpdatecolorsAddress             (m_MeshResource.colors.buffer                     );
		m_Desc.UpdatetexCoordsAddress          (m_MeshResource.texCoords.buffer                  );
		m_Desc.UpdateverticesAddress           (m_MeshResource.vertices.buffer                   );
		m_Desc.UpdateprimitivePointsAddress    (m_MeshResource.primitivePoints.buffer            );
		m_Desc.UpdateprimitiveVerticesAddress  (m_MeshResource.primitiveVertices.buffer          );
		m_Desc.UpdateprimitiveLocationsAddress (m_MeshResource.primitiveLocations.buffer         );
		m_Desc.UpdatemeshletsAddress           (m_MeshResource.meshlets.buffer                   );
		m_Desc.UpdatenMeshlets                 (m_MeshResource.meshlets.attributes       ->size());
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
				const uint32_t vtIndex = i * m_Columns + j;
				float colRamp = j / static_cast<float>(m_Columns - 1) - 0.5f; // -0.5f ~ 0.5f

				m_MeshResource.positions.attributes->push_back({ colRamp, rowRamp, 0.0f });
				m_MeshResource.normals  .attributes->push_back({ 0.0f, 0.0f, -1.0f });
				m_MeshResource.colors   .attributes->push_back({ 1.0f, 1.0f, 1.0f });
				m_MeshResource.texCoords.attributes->push_back({ colRamp + 0.5, 0.5 - rowRamp });

				m_MeshResource.vertices .attributes->push_back(glm::uvec4(vtIndex));
			}
		}

		for (uint32_t i = 0; i < m_Rows - 1; i++)
		{
			for (uint32_t j = 0; j < m_Columns - 1; j++)
			{
				const uint32_t vtIndex = i * m_Columns + j;
				
				m_MeshResource.primitiveVertices.attributes->push_back({ vtIndex, vtIndex + 1, vtIndex + m_Columns + 1 });
				m_MeshResource.primitiveVertices.attributes->push_back({ vtIndex + m_Columns + 1, vtIndex + m_Columns, vtIndex });
			}
		}
		
		if (isCreateBuffer)
		{
			MeshProcessor::GenerateMeshLodClusterHierarchy(this);
			CreateBuffer();
		}

		return true;
	}

	bool CubePack::OnCreatePack(bool isCreateBuffer)
	{
		SPICES_PROFILE_ZONE;

		if (MeshPack::OnCreatePack(isCreateBuffer)) return true;

		auto ApplyMatrixInPositions = [&](auto& positions, const glm::mat4 matrix) {
			
			for (uint64_t i = 0; i < positions.size(); i++)
			{
				glm::vec4 p  = matrix * glm::vec4(positions[i], 1.0f);
				positions[i] = { p.x, p.y, p.z };
			}
		};

		auto ApplyMatrixInNormals = [&](auto& normals, const glm::mat4 matrix) {

			for (uint64_t i = 0; i < normals.size(); i++)
			{
				glm::vec4 n = matrix * glm::vec4(normals[i], 1.0f);
				normals[i]  = { n.x, n.y, n.z };
			}
		};

		auto CopyToVertices = [&](auto& resources) {

			uint64_t nPositions = m_MeshResource.positions.attributes->size();
			m_MeshResource.positions.attributes->insert(
				m_MeshResource.positions.attributes->end(), 
				resources.positions.attributes->begin(),
				resources.positions.attributes->end()
			);

			uint64_t nNormals = m_MeshResource.normals.attributes->size();
			m_MeshResource.normals.attributes->insert(
				m_MeshResource.normals.attributes->end(),
				resources.normals.attributes->begin(),
				resources.normals.attributes->end()
			);

			uint64_t nColors = m_MeshResource.colors.attributes->size();
			m_MeshResource.colors.attributes->insert(
				m_MeshResource.colors.attributes->end(),
				resources.colors.attributes->begin(),
				resources.colors.attributes->end()
			);

			uint64_t nTexCoords = m_MeshResource.texCoords.attributes->size();
			m_MeshResource.texCoords.attributes->insert(
				m_MeshResource.texCoords.attributes->end(),
				resources.texCoords.attributes->begin(),
				resources.texCoords.attributes->end()
			);

			uint64_t nVertices = m_MeshResource.vertices.attributes->size();
			for (auto& vertex : *resources.vertices.attributes)
			{
				m_MeshResource.vertices.attributes->push_back({
						vertex.x + nPositions ,
						vertex.y + nNormals   ,
						vertex.z + nColors    ,
						vertex.w + nTexCoords
				});
			}

			uint64_t nPrimVertices = m_MeshResource.primitiveVertices.attributes->size();
			for (auto& primitiveVertex : *resources.primitiveVertices.attributes)
			{
				m_MeshResource.primitiveVertices.attributes->push_back({
					primitiveVertex.x + nVertices,
					primitiveVertex.y + nVertices,
					primitiveVertex.z + nVertices
				});
			}
		};

		// Front
		{
			PlanePack pack(m_Rows, m_Columns);
			pack.OnCreatePack(false);
			glm::mat4 tran = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f));

			auto& resources = pack.GetResource();

			auto positions = resources.positions.attributes;
			ApplyMatrixInPositions(*positions, tran);

			auto normals = resources.normals.attributes;
			ApplyMatrixInNormals(*normals, glm::mat4(1.0f));

			CopyToVertices(resources);
		}
		
		// Back
		{
			PlanePack pack(m_Rows, m_Columns);
			pack.OnCreatePack(false);
			glm::mat4 tran = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.5f));
			glm::mat4 rot = glm::toMat4(glm::quat({0.0f, glm::radians(180.0f), 0.0f}));

			auto& resources = pack.GetResource();

			auto positions = resources.positions.attributes;
			ApplyMatrixInPositions(*positions, tran * rot);

			auto normals = resources.normals.attributes;
			ApplyMatrixInNormals(*normals, glm::mat4(1.0f));

			CopyToVertices(resources);			
		}
		
		// Left
		{
			PlanePack pack(m_Rows, m_Columns);
			pack.OnCreatePack(false);
			glm::mat4 tran = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));
			glm::mat4 rot = glm::toMat4(glm::quat({ 0.0f, glm::radians(-90.0f), 0.0f }));

			auto& resources = pack.GetResource();

			auto positions = resources.positions.attributes;
			ApplyMatrixInPositions(*positions, tran * rot);

			auto normals = resources.normals.attributes;
			ApplyMatrixInNormals(*normals, glm::mat4(1.0f));

			CopyToVertices(resources);			
		}
		
		// Right
		{
			PlanePack pack(m_Rows, m_Columns);
			pack.OnCreatePack(false);
			glm::mat4 tran = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, 0.0f));
			glm::mat4 rot = glm::toMat4(glm::quat({ 0.0f, glm::radians(90.0f), 0.0f }));

			auto& resources = pack.GetResource();

			auto positions = resources.positions.attributes;
			ApplyMatrixInPositions(*positions, tran * rot);

			auto normals = resources.normals.attributes;
			ApplyMatrixInNormals(*normals, glm::mat4(1.0f));

			CopyToVertices(resources);
		}
		
		// Top
		{
			PlanePack pack(m_Rows, m_Columns);
			pack.OnCreatePack(false);
			glm::mat4 tran = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f));
			glm::mat4 rot = glm::toMat4(glm::quat({ glm::radians(-90.0f), 0.0f, 0.0f }));

			auto& resources = pack.GetResource();

			auto positions = resources.positions.attributes;
			ApplyMatrixInPositions(*positions, tran * rot);

			auto normals = resources.normals.attributes;
			ApplyMatrixInNormals(*normals, glm::mat4(1.0f));

			CopyToVertices(resources);
		}
		
		// Button
		{
			PlanePack pack(m_Rows, m_Columns);
			pack.OnCreatePack(false);
			glm::mat4 tran = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.0f));
			glm::mat4 rot = glm::toMat4(glm::quat({ glm::radians(90.0f), 0.0f, 0.0f }));

			auto& resources = pack.GetResource();

			auto positions = resources.positions.attributes;
			ApplyMatrixInPositions(*positions, tran * rot);

			auto normals = resources.normals.attributes;
			ApplyMatrixInNormals(*normals, glm::mat4(1.0f));

			CopyToVertices(resources);
		}
		
		if (isCreateBuffer)
		{
			MeshProcessor::GenerateMeshLodClusterHierarchy(this);
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
				const uint32_t vtIndex = i * m_Columns + j;
				const float colRamp = j * 360.0f / static_cast<float>(m_Columns - 1); // 0 ~ 360

				glm::vec3 position{ glm::sin(glm::radians(rowRamp)) * glm::sin(glm::radians(colRamp)), glm::cos(glm::radians(rowRamp)), glm::sin(glm::radians(rowRamp)) * glm::cos(glm::radians(colRamp)) };

				m_MeshResource.positions.attributes->push_back(position);
				m_MeshResource.normals  .attributes->push_back(glm::normalize(position));
				m_MeshResource.colors   .attributes->push_back({ 1.0f, 1.0f, 1.0f });
				m_MeshResource.texCoords.attributes->push_back({ j / static_cast<float>(m_Columns - 1), i / static_cast<float>(m_Rows - 1) });

				m_MeshResource.vertices .attributes->push_back(glm::uvec4(vtIndex));
			}
		}
		
		for (uint32_t i = 0; i < m_Rows - 1; i++)
		{
			for (uint32_t j = 0; j < m_Columns - 1; j++)
			{
				const uint32_t vtIndex = i * m_Columns + j;

				m_MeshResource.primitiveVertices.attributes->push_back({ vtIndex, vtIndex + 1, vtIndex + m_Columns + 1 });
				m_MeshResource.primitiveVertices.attributes->push_back({ vtIndex + m_Columns + 1, vtIndex + m_Columns, vtIndex });
			}
		}

		if (isCreateBuffer)
		{
			MeshProcessor::GenerateMeshLodClusterHierarchy(this);
			CreateBuffer();
		}

		return true;
	}

}