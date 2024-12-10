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
	
	std::unique_ptr<VulkanRayTracing> RayTracingRenderer::m_VulkanRayTracing = nullptr;

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
	}

	RayTracingRenderer::~RayTracingRenderer()
	{
		SPICES_PROFILE_ZONE;

		m_VulkanRayTracing = nullptr;
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
		* @brief Wait for last frame render finish.
		* So we can recreate BLAS/TLAS safely.
		*/
		{
			SPICES_PROFILE_ZONEN("Wait for GPU Work finish");

			vkDeviceWaitIdle(m_VulkanState.m_Device);
		}

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
		* @breif ReCreate DefaultMaterial.
		*/
		CreateDefaultMaterial();

		CreateRTShaderBindingTable();
	}

	void RayTracingRenderer::CreatePipeline(
		std::shared_ptr<Material>        material ,
		VkPipelineLayout&                layout   ,
		std::shared_ptr<RendererSubPass> subPass
	)
	{
		SPICES_PROFILE_ZONE;

		PipelineBuilder{ subPass, material, this }
		.SetDefault()
		.NullBindingDescriptions()
		.NullAttributeDescriptions()
		.SetPipelineLayout(layout)
		.BuildRayTracing(m_VulkanRayTracing->GetHitGroups());
	}

	void RayTracingRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		if (m_VulkanRayTracing->GetAccelerationStructure() == VK_NULL_HANDLE) return;

		UpdateTopLevelAS(frameInfo);

		if(frameInfo.m_RendererType != RendererType::PathTracing) return;
		
		RayTracingRenderBehaveBuilder builder{ this, m_VulkanRayTracing.get(), frameInfo.m_FrameIndex, frameInfo.m_ImageIndex};

		builder.BeginRenderPass();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("RayTracing"));
		
		builder.BindPipeline("RayTracingRenderer.RayTracing.Default");

		builder.UpdateAccelerationStructure(2, 0, m_VulkanRayTracing->GetAccelerationStructure());

		builder.UpdateStorageBuffer(3, 0, m_VulkanRayTracing->GetMeshDescBuffer().get());
		
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
		
		builder.TraceRays();

		builder.EndRenderPass();
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
		m_VulkanRayTracing->BuildBLAS(
			allBlas, 
			VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR | 
			VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR 
			//VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_COMPACTION_BIT_KHR     // Compress cost too mush time in with large count of meshes.
		);
	}
 
	void RayTracingRenderer::CreateTopLevelAS(FrameInfo& frameInfo, bool update)
	{
		SPICES_PROFILE_ZONE;

		std::vector<VkAccelerationStructureInstanceKHR> tlas;

		int index = 0;
		std::shared_ptr<RayTracingR::MeshDescBuffer> descBuffer = std::make_shared<RayTracingR::MeshDescBuffer>();

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
				rayInst.mask                                                = 0xFF;                                                       // Only be hit if rayMask & instance.mask != 0
				rayInst.instanceShaderBindingTableRecordOffset              = v->GetHitShaderHandle();                                    // We will use the same hit group for all objects

				tlas.push_back(rayInst);

				descBuffer->descs[index] = v->GetMeshDesc().GetBufferAddress();

				index += 1;
				return false;
			});
		}
		
		m_VulkanRayTracing->SetMeshDescBuffer(descBuffer);

		/**
		* @brief Build TLAS.
		*/
		m_VulkanRayTracing->BuildTLAS(
			tlas,
			VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR |
			VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR      |
			VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_COMPACTION_BIT_KHR  ,
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

	void RayTracingRenderer::CreateRTShaderBindingTable()
	{
		SPICES_PROFILE_ZONE;

		const auto rayTracingMaterial = ResourcePool<Material>::Load<Material>("RayTracingRenderer.RayTracing.Default", "RayTracingRenderer.RayTracing.Default");
								  
		const uint32_t rayGenCount    = static_cast<uint32_t>(rayTracingMaterial->GetShaderPath("rgen").size());
		const uint32_t missCount      = static_cast<uint32_t>(rayTracingMaterial->GetShaderPath("rmiss").size());
		 
		m_VulkanRayTracing->CreateRTShaderBindingTable(rayGenCount, missCount, m_Pipelines["RayTracingRenderer.RayTracing.Default"]->GetPipeline());
	}
}