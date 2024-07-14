/**
* @file RayTracingRenderer.cpp.
* @brief The RayTracingRenderer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "RayTracingRenderer.h"
#include "PreRenderer.h"
#include "Render/Vulkan/VulkanRayTracing.h"
#include "Core/Library/MemoryLibrary.h"

namespace Spices {
	
	RayTracingRenderer::RayTracingRenderer(
		const std::string&                           rendererName          ,
		VulkanState&                                 vulkanState           ,
		const std::shared_ptr<VulkanDescriptorPool>& descriptorPool        ,
		const std::shared_ptr<VulkanDevice>&         device                ,
		const std::shared_ptr<RendererResourcePool>& rendererResourcePool
	)
		: Renderer(rendererName, vulkanState, descriptorPool, device, rendererResourcePool)
	{
		SPICES_PROFILE_ZONE;

		m_VulkanRayTracing = std::make_unique<VulkanRayTracing>(m_VulkanState);

		vkGetRayTracingShaderGroupHandlesKHR  = reinterpret_cast<PFN_vkGetRayTracingShaderGroupHandlesKHR>(vkGetInstanceProcAddr(vulkanState.m_Instance, "vkGetRayTracingShaderGroupHandlesKHR"));
	}

	void RayTracingRenderer::CreateRendererPass()
	{
		SPICES_PROFILE_ZONE;

		RendererPassBuilder{ "RayTracing", this }
		.AddSubPass("RayTracing")
		.EndSubPass()
		.Build();
	}

	void RayTracingRenderer::CreateDescriptorSet()
	{
		SPICES_PROFILE_ZONE;

		DescriptorSetBuilder{ "RayTracing", this }
		.AddAccelerationStructure(1, 0, VK_SHADER_STAGE_RAYGEN_BIT_KHR | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR)   /* @brief Acceleration Structure.         */
		.AddStorageTexture(1, 1, VK_SHADER_STAGE_RAYGEN_BIT_KHR, { "Ray" }, VK_FORMAT_R32G32B32A32_SFLOAT)      /* @brief Ray Tracing Output Image.       */
		.AddStorageBuffer<RayTracingR::MeshDescBuffer>(1, 2, VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR)               /* @brief World Mesh Buffer.              */
		.AddStorageBuffer<RayTracingR::MaterialParameterBuffer>(1, 3, VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR)               /* @brief World Mesh Buffer.              */
		.AddStorageBuffer<RayTracingR::DirectionalLightBuffer>(1, 4, VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR)       /* @brief World Directional Light Buffer. */
		.AddStorageBuffer<RayTracingR::PointLightBuffer>(1, 5, VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR)             /* @brief World PointLight Buffer.        */
		.AddTexture<Texture2D>(2, 0, VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR, {"interior_stair_wl3ieamdw/wl3ieamdw_4K_Albedo.jpg"})  /* @brief temp */
		.AddTexture<Texture2D>(2, 1, VK_SHADER_STAGE_MISS_BIT_KHR, {"skybox/kloofendal_48d_partly_cloudy_puresky_4k.hdr "})                                    /* @brief temp */
		.Build(m_VulkanRayTracing->GetAccelerationStructure());
	}

	void RayTracingRenderer::OnMeshAddedWorld()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Destroy old AS if created before.
		*/
		m_VulkanRayTracing->Destroy();

		/**
		* @brief Prepare RayTracing AC Structure ans SBT.
		*/
		CreateBottomLevelAS(FrameInfo::Get());
		CreateTopLevelAS(FrameInfo::Get());

		/**
		* @breif ReCreate RenderPass, DescriptorSet and DefaultMaterial.
		*/
		Renderer::OnSystemInitialize();

		CreateRTShaderBindingTable(FrameInfo::Get());
	}

	std::shared_ptr<VulkanPipeline> RayTracingRenderer::CreatePipeline(
		std::shared_ptr<Material>         material ,
		VkPipelineLayout&                 layout   ,
		std::shared_ptr<RendererSubPass>  subPass
	)
	{
		SPICES_PROFILE_ZONE;

		PipelineConfigInfo pipelineConfig{};

		pipelineConfig.pipelineLayout              = layout;

		std::unordered_map<std::string, std::vector<std::string>> stages(material->GetShaderPath());
		for (auto& pair : m_HitGroups)
		{
			stages["rchit"].push_back(pair.first);
		}

		return std::make_shared<VulkanRayTracingPipeline>(
			m_VulkanState,
			material->GetName(),
			stages,
			pipelineConfig
		);
	}

	void RayTracingRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		if (m_VulkanRayTracing->GetAccelerationStructure() == VK_NULL_HANDLE) return;

		CreateTopLevelAS(frameInfo, true);
		
		RayTracingRenderBehaveBuilder builder{ this , frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.Recording("RayTracing");

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("RayTracing"));
		
		builder.BindPipeline("RayTracingRenderer.RayTracing.Default");

		builder.UpdateStorageBuffer(1, 2, m_DescArray.get());

		builder.UpdateStorageBuffer(1, 3, m_ParamArray.get());
		
		builder.UpdateStorageBuffer<RayTracingR::DirectionalLightBuffer>(1, 4, [&](auto& ssbo) {
			GetDirectionalLight(frameInfo, ssbo.lights);
		});
		
		builder.UpdateStorageBuffer<RayTracingR::PointLightBuffer>(1, 5, [&](auto& ssbo) {
			GetPointLight(frameInfo, ssbo.lights);
		});

		builder.TraceRays(&m_RgenRegion, &m_MissRegion, &m_HitRegion, &m_CallRegion);

		builder.Endrecording();
	}

	void RayTracingRenderer::CreateBottomLevelAS(FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief BLAS - Storing each primitive in a geometry.
		*/
		std::vector<VulkanRayTracing::BlasInput> allBlas;
		m_HitGroups.clear();
		
		/**
		* @brief Iter all MeshComponents.
		*/
		auto view = frameInfo.m_World->GetRegistry().view<MeshComponent>();
		for (auto& e : view)
		{
			auto& meshComp = frameInfo.m_World->GetRegistry().get<MeshComponent>(e);

			auto blas = meshComp.GetMesh()->CreateMeshPackASInput();
			ContainerLibrary::Append<VulkanRayTracing::BlasInput>(allBlas, blas);

			meshComp.GetMesh()->AddMaterialToHitGroup(m_HitGroups);
		}

		/**
		* @brief Build BLAS.
		*/
		m_VulkanRayTracing->BuildBLAS(allBlas, VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR | VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR | VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_COMPACTION_BIT_KHR);
	}

	void RayTracingRenderer::CreateTopLevelAS(FrameInfo& frameInfo, bool update)
	{
		SPICES_PROFILE_ZONE;

		std::vector<VkAccelerationStructureInstanceKHR> tlas;

		int index = 0;
		m_DescArray = std::make_unique<RayTracingR::MeshDescBuffer>();
		m_ParamArray = std::make_unique<RayTracingR::MaterialParameterBuffer>();
		auto view = frameInfo.m_World->GetRegistry().view<MeshComponent>();
		for (auto& e : view)
		{
			auto [meshComp, tranComp] = frameInfo.m_World->GetRegistry().get<MeshComponent, TransformComponent>(e);

			for(auto& pair : meshComp.GetMesh()->GetPacks())
			{
				VkAccelerationStructureInstanceKHR rayInst{};
				rayInst.transform                                           = ToVkTransformMatrixKHR(tranComp.GetModelMatrix());                    // Position of the instance
				rayInst.instanceCustomIndex                                 = index;                                                      // gl_InstanceCustomIndexEXT
				rayInst.accelerationStructureReference                      = m_VulkanRayTracing->GetBlasDeviceAddress(index);
				rayInst.flags                                               = VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR;
				rayInst.mask                                                = 0xFF;                                                       //  Only be hit if rayMask & instance.mask != 0
				rayInst.instanceShaderBindingTableRecordOffset              = pair.second->GetMaterialHandle();                           // We will use the same hit group for all objects

				tlas.push_back(rayInst);

				m_DescArray->descs[index].vertexAddress = pair.second->GetVerticesBufferAddress();
				m_DescArray->descs[index].indexAddress  = pair.second->GetIndicesBufferAddress();
				m_ParamArray->params[index] = pair.second->GetMaterial()->GetConstantParamsAddress();
				
				index += 1;
			}
		}
		
		/**
		* @brief Build TLAS.
		*/
		m_VulkanRayTracing->BuildTLAS(
			tlas,
			VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR |
			VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR |
			VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_COMPACTION_BIT_KHR,
			update
		);
	}
	
	void RayTracingRenderer::CreateRTShaderBindingTable(FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		const auto rayTracingMaterial                 = ResourcePool<Material>::Load<Material>("RayTracingRenderer.RayTracing.Default");

		const uint32_t rayGenCount              = rayTracingMaterial->GetShaderPath("rgen").size();
		const uint32_t missCount                = rayTracingMaterial->GetShaderPath("rmiss").size();
		const uint32_t hitCount                 = static_cast<uint32_t>(m_HitGroups.size());

		const auto handleCount          = rayGenCount + missCount + hitCount;
		uint32_t handleSize                     = m_Device->GetRTPipelineProperties().shaderGroupHandleSize;

		/**
		* @brief The SBT(buffer) need to have starting groups to be aligned and handles in the group to be aligned.
		*/
		const uint32_t handleSizeAligned              = MemoryLibrary::align_up(handleSize, m_Device->GetRTPipelineProperties().shaderGroupHandleAlignment);

		m_RgenRegion.stride                     = MemoryLibrary::align_up(handleSizeAligned, m_Device->GetRTPipelineProperties().shaderGroupBaseAlignment);
		m_RgenRegion.size                       = m_RgenRegion.stride;  // The size member of pRayGenShaderBindingTable must be equal to its stride member

		m_MissRegion.stride                     = handleSizeAligned;
		m_MissRegion.size                       = MemoryLibrary::align_up(missCount * handleSizeAligned, m_Device->GetRTPipelineProperties().shaderGroupBaseAlignment);

		m_HitRegion.stride                      = handleSizeAligned;
		m_HitRegion.size                        = MemoryLibrary::align_up(hitCount * handleSizeAligned, m_Device->GetRTPipelineProperties().shaderGroupBaseAlignment);

		/**
		* @brief Get the shader group handles.
		*/
		const uint32_t dataSize                 = handleCount * handleSize;
		std::vector<uint8_t> handles(dataSize);
		VK_CHECK(vkGetRayTracingShaderGroupHandlesKHR(m_VulkanState.m_Device, m_Pipelines["RayTracingRenderer.RayTracing.Default"]->GetPipeline(), 0, handleCount, dataSize, handles.data()))

		/**
		* @brief Allocate a buffer for storing the SBT.
		*/ 
		VkDeviceSize sbtSize = m_RgenRegion.size + m_MissRegion.size + m_HitRegion.size + m_CallRegion.size;

		m_RTSBTBuffer = std::make_unique<VulkanBuffer>(
			m_VulkanState, 
			sbtSize, 
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT | 
			VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | 
			VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR, 
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		);

		VulkanDebugUtils::SetObjectName(VK_OBJECT_TYPE_BUFFER, m_RTSBTBuffer->Get(), m_VulkanState.m_Device, "SBT Buffer");

		m_RgenRegion.deviceAddress              = m_RTSBTBuffer->GetAddress();
		m_MissRegion.deviceAddress              = m_RTSBTBuffer->GetAddress() + m_RgenRegion.size;
		m_HitRegion.deviceAddress               = m_RTSBTBuffer->GetAddress() + m_RgenRegion.size + m_MissRegion.size;

		/**
		* @brief Helper to retrieve the handle data.
		*/ 
		auto getHandle = [&](int i) { return handles.data() + i * handleSize; };

		void* data;
		vkMapMemory(m_VulkanState.m_Device, m_RTSBTBuffer->GetMemory(), 0, sbtSize, 0, &data);

		/**
		* @brief Map the SBT buffer and write in the handles.
		*/
		uint8_t* pSBTBuffer                     = reinterpret_cast<uint8_t*>(data);
		uint8_t* pData                          = nullptr;
		uint32_t handleIdx                      = 0 ;

		/**
		* @brief Ray Generation.
		*/ 
		pData = pSBTBuffer;
		for (uint32_t c = 0; c < rayGenCount; c++)
		{
			memcpy(pData, getHandle(handleIdx++), handleSize);
			m_RgenRegion.stride;
		}

		/**
		* @brief Miss.
		*/ 
		pData = pSBTBuffer + m_RgenRegion.size;
		for (uint32_t c = 0; c < missCount; c++)
		{
			memcpy(pData, getHandle(handleIdx++), handleSize);
			pData += m_MissRegion.stride;
		}

		/**
		* @brief Closest Hit.
		*/ 
		pData = pSBTBuffer + m_RgenRegion.size + m_MissRegion.size;
		for (uint32_t c = 0; c < hitCount; c++)
		{
			memcpy(pData, getHandle(handleIdx++), handleSize);
			pData += m_HitRegion.stride;
		}

		vkUnmapMemory(m_VulkanState.m_Device, m_RTSBTBuffer->GetMemory());
	}
}