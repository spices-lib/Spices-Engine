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
		const std::shared_ptr<RendererResourcePool>& rendererResourcePool  ,
		const std::shared_ptr<VulkanCmdThreadPool>&  cmdThreadPool
	)
		: Renderer(rendererName, vulkanState, descriptorPool, device, rendererResourcePool, cmdThreadPool, true, false)
	{
		SPICES_PROFILE_ZONE;

		m_VulkanRayTracing = std::make_unique<VulkanRayTracing>(m_VulkanState);
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
		.AddPushConstant(sizeof(RayTracingR::PushConstant))
		.AddAccelerationStructure(2, 0, VK_SHADER_STAGE_RAYGEN_BIT_KHR | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR)                    /* @brief Acceleration Structure.         */
		.AddStorageTexture(2, 1, VK_SHADER_STAGE_RAYGEN_BIT_KHR, { "RayImage" }, VK_FORMAT_R32G32B32A32_SFLOAT)                  /* @brief Ray Tracing Output Image.       */
		.AddStorageTexture(2, 2, VK_SHADER_STAGE_RAYGEN_BIT_KHR, { "RayEntityID", "RayTriangleID" }, VK_FORMAT_R32_SFLOAT)       /* @brief Ray Tracing Output IDs.         */
		.AddStorageBuffer(3, 0, sizeof(RayTracingR::MeshDescBuffer), VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR)                        /* @brief World Mesh Buffer.              */
		.AddStorageBuffer(3, 1, sizeof(RayTracingR::DirectionalLightBuffer), VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR)                /* @brief World Directional Light Buffer. */
		.AddStorageBuffer(3, 2, sizeof(RayTracingR::PointLightBuffer), VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR)                      /* @brief World PointLight Buffer.        */
		.AddTexture<Texture2D>(4, 0, VK_SHADER_STAGE_MISS_BIT_KHR, {"skybox/kloofendal_48d_partly_cloudy_puresky_4k.hdr"})       /* @brief temp.                           */
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

		if(frameInfo.m_RendererType != RendererType::PathTracing) return;
		
		if (m_VulkanRayTracing->GetAccelerationStructure() == VK_NULL_HANDLE) return;

		UpdateTopLevelAS(frameInfo);
		
		RayTracingRenderBehaveBuilder builder{ this , frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.Recording("RayTracing");

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("RayTracing"));
		
		builder.BindPipeline("RayTracingRenderer.RayTracing.Default");

		builder.UpdateStorageBuffer(3, 0, m_DescArray.get());
		
		builder.UpdateStorageBuffer<RayTracingR::DirectionalLightBuffer>(3, 1, [&](auto& ssbo) {
			GetDirectionalLight(frameInfo, ssbo.lights);
		});
		
		builder.UpdateStorageBuffer<RayTracingR::PointLightBuffer>(3, 2, [&](auto& ssbo) {
			GetPointLight(frameInfo, ssbo.lights);
		});

		builder.UpdatePushConstant<RayTracingR::PushConstant>([&](auto& push) {
			IterWorldCompWithBreak<SkyBoxComponent>(frameInfo, [&](int entityId, TransformComponent& transComp, SkyBoxComponent& camComp){
				push.entityID = entityId;
				return true;
			});
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
			allBlas.insert(allBlas.end(), blas.begin(), blas.end());

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
		auto view = frameInfo.m_World->GetRegistry().view<MeshComponent>();
		for (auto& e : view)
		{
			MeshComponent meshComp;
			TransformComponent tranComp;

			std::tie(meshComp, tranComp) = frameInfo.m_World->GetRegistry().get<MeshComponent, TransformComponent>(e);

			meshComp.GetMesh()->GetPacks().for_each([&](const uint32_t& k, const std::shared_ptr<MeshPack>& v) {

				VkAccelerationStructureInstanceKHR                            rayInst{};
				rayInst.transform                                           = ToVkTransformMatrixKHR(tranComp.GetModelMatrix());          // Position of the instance
				rayInst.instanceCustomIndex                                 = index;                                                      // gl_InstanceCustomIndexEXT
				rayInst.accelerationStructureReference                      = m_VulkanRayTracing->GetBlasDeviceAddress(index);
				rayInst.flags                                               = VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR;
				rayInst.mask                                                = 0xFF;                                                       //  Only be hit if rayMask & instance.mask != 0
				rayInst.instanceShaderBindingTableRecordOffset              = v->GetHitShaderHandle();                                    // We will use the same hit group for all objects

				tlas.push_back(rayInst);

				m_DescArray->descs[index] = v->GetMeshDesc().GetBufferAddress();

				index += 1;
				return false;
			});
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
	
	void RayTracingRenderer::UpdateTopLevelAS(FrameInfo& frameInfo, bool update)
	{
		SPICES_PROFILE_ZONE;

		if(!(frameInfo.m_World->GetMarker() & World::NeedUpdateTLAS)) return;
		frameInfo.m_World->ClearMarkerWithBits(World::NeedUpdateTLAS);
		
		CreateTopLevelAS(frameInfo, update);
	}

	void RayTracingRenderer::CreateRTShaderBindingTable(FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		const auto rayTracingMaterial           = ResourcePool<Material>::Load<Material>("RayTracingRenderer.RayTracing.Default", "RayTracingRenderer.RayTracing.Default");

		const uint32_t rayGenCount              = static_cast<uint32_t>(rayTracingMaterial->GetShaderPath("rgen").size());
		const uint32_t missCount                = static_cast<uint32_t>(rayTracingMaterial->GetShaderPath("rmiss").size());
		const uint32_t hitCount                 = static_cast<uint32_t>(m_HitGroups.size());

		const auto handleCount                  = rayGenCount + missCount + hitCount;
		uint32_t handleSize                     = m_Device->GetRTPipelineProperties().shaderGroupHandleSize;

		/**
		* @brief The SBT(buffer) need to have starting groups to be aligned and handles in the group to be aligned.
		*/
		const uint32_t handleSizeAligned        = MemoryLibrary::align_up(handleSize, m_Device->GetRTPipelineProperties().shaderGroupHandleAlignment);

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
		VK_CHECK(m_VulkanState.m_VkFunc.vkGetRayTracingShaderGroupHandlesKHR(m_VulkanState.m_Device, m_Pipelines["RayTracingRenderer.RayTracing.Default"]->GetPipeline(), 0, handleCount, dataSize, handles.data()))

		/**
		* @brief Allocate a buffer for storing the SBT.
		*/ 
		VkDeviceSize sbtSize = m_RgenRegion.size + m_MissRegion.size + m_HitRegion.size + m_CallRegion.size;

		m_RTSBTBuffer = std::make_unique<VulkanBuffer>(
			m_VulkanState, 
			"SBTBuffer",
			sbtSize, 
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT             | 
			VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT    | 
			VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR , 
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT          | 
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		);

		DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_BUFFER, (uint64_t)m_RTSBTBuffer->Get(), m_VulkanState.m_Device, "SBT Buffer")

		m_RgenRegion.deviceAddress              = m_RTSBTBuffer->GetAddress();
		m_MissRegion.deviceAddress              = m_RTSBTBuffer->GetAddress() + m_RgenRegion.size;
		m_HitRegion.deviceAddress               = m_RTSBTBuffer->GetAddress() + m_RgenRegion.size + m_MissRegion.size;

		/**
		* @brief Helper to retrieve the handle data.
		*/ 
		auto getHandle = [&](int i) { return handles.data() + i * handleSize; };

		/**
		* @brief Map the SBT buffer and write in the handles.
		*/
		uint64_t offset                         = 0;
		uint32_t handleIdx                      = 0 ;

		/**
		* @brief Ray Generation.
		*/ 
		for (uint32_t c = 0; c < rayGenCount; c++)
		{
			m_RTSBTBuffer->WriteToBuffer(getHandle(handleIdx++), handleSize, offset);
			m_RgenRegion.stride;
		}

		/**
		* @brief Miss.
		*/ 
		offset = m_RgenRegion.size;
		for (uint32_t c = 0; c < missCount; c++)
		{
			m_RTSBTBuffer->WriteToBuffer(getHandle(handleIdx++), handleSize, offset);
			offset += m_MissRegion.stride;
		}

		/**
		* @brief Closest Hit.
		*/ 
		offset = m_RgenRegion.size + m_MissRegion.size;
		for (uint32_t c = 0; c < hitCount; c++)
		{
			m_RTSBTBuffer->WriteToBuffer(getHandle(handleIdx++), handleSize, offset);
			offset += m_HitRegion.stride;
		}
	}
}