/**
* @file RayTracingRenderer.cpp.
* @brief The RayTracingRenderer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "RayTracingRenderer.h"
#include "PreRenderer.h"
#include "Render/Vulkan/VulkanRayTracing.h"

namespace Spices {
	
	std::shared_ptr<VulkanRayTracing> RayTracingRenderer::m_VulkanRayTracing = nullptr;

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

		m_VulkanRayTracing = std::make_shared<VulkanRayTracing>(m_VulkanState);
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
		.AddStorageBuffer(3, 0, SpicesShader::MESH_BUFFER_MAXNUM * sizeof(uint64_t), VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR)        /* @brief World Mesh Buffer.              */
		.AddStorageBuffer(3, 1, sizeof(RayTracingR::DirectionalLightBuffer), VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR)                /* @brief World Directional Light Buffer. */
		.AddStorageBuffer(3, 2, sizeof(RayTracingR::PointLightBuffer), VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR)                      /* @brief World PointLight Buffer.        */
		.AddTexture<Texture2D>(4, 0, VK_SHADER_STAGE_MISS_BIT_KHR, {"skybox/kloofendal_48d_partly_cloudy_puresky_4k.hdr"})       /* @brief temp.                           */
		.Build(m_VulkanRayTracing->GetAccelerationStructure());
	}
	
	void RayTracingRenderer::OnMeshAddedWorld()
	{
		Renderer::OnMeshAddedWorld();

		auto view = GetEntityWithComponent<MeshComponent>(FrameInfo::Get().m_World.get());
		
		AsyncTask(ThreadPoolEnum::Custom, [=]() {

			SPICES_PROFILE_ZONEN("RayTracingRenderer::OnMeshAddedWorld");

			std::shared_ptr<VulkanRayTracing> rayTracingInstance = std::make_shared<VulkanRayTracing>(m_VulkanState);

			/**
			* @brief Create BLAS/TLAS.
			*/
			CreateBottomLevelAS(FrameInfo::Get(), view, rayTracingInstance);
			CreateTopLevelAS   (FrameInfo::Get(), view, rayTracingInstance);

			/**
			* @brief Create Pipeline/SBT.
			*/
			CreateDefaultMaterial();
			CreateRTShaderBindingTable(rayTracingInstance);

			/**
			* @brief Submit new raytracing instance.
			*/
			AsyncMainTask(ThreadPoolEnum::Main, [=]() {

				vkQueueWaitIdle(m_VulkanState.m_GraphicQueue);
				m_RenderCache->PushToCaches(m_VulkanRayTracing);
				m_VulkanRayTracing = rayTracingInstance;

			});
		});
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
		.BuildRayTracing(*GetHitGroupsCache());
	}

	void RayTracingRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		if (m_VulkanRayTracing->GetAccelerationStructure() == VK_NULL_HANDLE) return;

		UpdateTopLevelAS(frameInfo, m_VulkanRayTracing);

		if(frameInfo.m_RendererType != RendererType::PathTracing) return;
		
		RayTracingRenderBehaveBuilder builder{ this, m_VulkanRayTracing.get(), frameInfo.m_FrameIndex, frameInfo.m_ImageIndex};

		builder.BeginRenderPass();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("RayTracing"));
		
		builder.BindPipeline("RayTracingRenderer.RayTracing.Default");

		builder.UpdateAccelerationStructure(2, 0, m_VulkanRayTracing->GetAccelerationStructure());

		builder.UpdateStorageBuffer(3, 0, m_VulkanRayTracing->GetMeshDesc().buffer);

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

	VkAccelerationStructureKHR RayTracingRenderer::GetAccelerationStructure()
	{
		SPICES_PROFILE_ZONE;

		if(!m_VulkanRayTracing) return nullptr;
		
		return m_VulkanRayTracing->GetAccelerationStructure();
	}

	void RayTracingRenderer::UpdateTopLevelAS(FrameInfo& frameInfo, std::shared_ptr<VulkanRayTracing> rayTracingInstance, bool update)
	{
		SPICES_PROFILE_ZONE;

		if(!(frameInfo.m_World->GetMarker() & World::NeedUpdateTLAS)) return;
		frameInfo.m_World->ClearMarkerWithBits(World::NeedUpdateTLAS);
		
		auto view = GetEntityWithComponent<MeshComponent>(frameInfo.m_World.get());
		CreateTopLevelAS(frameInfo, view, rayTracingInstance, update);
	}

	void RayTracingRenderer::CreateRTShaderBindingTable(std::shared_ptr<VulkanRayTracing> rayTracingInstance)
	{
		SPICES_PROFILE_ZONE;

		const auto rayTracingMaterial = ResourcePool<Material>::Load<Material>("RayTracingRenderer.RayTracing.Default", "RayTracingRenderer.RayTracing.Default");
								  
		const uint32_t rayGenCount    = static_cast<uint32_t>(rayTracingMaterial->GetShaderPath("rgen").size());
		const uint32_t missCount      = static_cast<uint32_t>(rayTracingMaterial->GetShaderPath("rmiss").size());
		 
		rayTracingInstance->CreateRTShaderBindingTable(rayGenCount, missCount, m_Pipelines.Find("RayTracingRenderer.RayTracing.Default")->GetPipeline());
	}

	std::shared_ptr<std::unordered_map<std::string, uint32_t>> RayTracingRenderer::GetHitGroupsCache()
	{
		SPICES_PROFILE_ZONE;

		if (!m_HitGroupsCache)
		{
			m_HitGroupsCache = std::make_shared<std::unordered_map<std::string, uint32_t>>();
		}
		
		return m_HitGroupsCache;
	}

	void RayTracingRenderer::CreateBottomLevelAS(FrameInfo& frameInfo, std::shared_ptr<std::vector<uint32_t>> view, std::shared_ptr<VulkanRayTracing> rayTracingInstance)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief BLAS - Storing each primitive in a geometry.
		*/
		std::vector<VulkanRayTracing::BlasInput> allBlas;
		
		/**
		* @brief Iter all MeshComponents.
		*/
		frameInfo.m_World->ViewComponent<MeshComponent>(*view, [&](auto e, auto& meshComp){

			meshComp.GetMesh()->GetPacks().for_each([&](const uint32_t& k, const std::shared_ptr<MeshPack>& v) {

				if (v->HasBlasAccel()) return false;

				auto blas = v->MeshPackToVkGeometryKHR();
				allBlas.emplace_back(blas);

				return false;
			});

			return false;
		});

		/**
		* @brief Skip if no blas build requirment.
		*/
		if(allBlas.empty()) return;
		
		/**
		* @brief Build BLAS.
		*/
		rayTracingInstance->BuildBLAS(
			allBlas, 
			VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR | 
			VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR 
			//VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_COMPACTION_BIT_KHR     // Compress cost too mush time in with large count of meshes.
		);
	}
 
	void RayTracingRenderer::CreateTopLevelAS(FrameInfo& frameInfo, std::shared_ptr<std::vector<uint32_t>> view, std::shared_ptr<VulkanRayTracing> rayTracingInstance, bool update)
	{
		SPICES_PROFILE_ZONE;

		std::vector<VkAccelerationStructureInstanceKHR> tlas;
		std::shared_ptr<std::unordered_map<std::string, uint32_t>> hitGroups = std::make_shared<std::unordered_map<std::string, uint32_t>>();

		int index = 0;
		auto& desc = rayTracingInstance->GetMeshDesc().attributes;
		desc->resize(SpicesShader::MESH_BUFFER_MAXNUM, 0);

		frameInfo.m_World->ViewComponent<MeshComponent>(*view, [&](auto e, auto& meshComp){
			
			auto& tranComp = frameInfo.m_World->GetComponent<TransformComponent>(static_cast<entt::entity>(e));

			meshComp.GetMesh()->AddMaterialToHitGroup(*hitGroups);
			meshComp.GetMesh()->GetPacks().for_each([&](const uint32_t& k, const std::shared_ptr<MeshPack>& v) {

				VkAccelerationStructureInstanceKHR                            rayInst{};
				rayInst.transform                                           = ToVkTransformMatrixKHR(tranComp.GetModelMatrix());          // Position of the instance
				rayInst.instanceCustomIndex                                 = index;                                                      // gl_InstanceCustomIndexEXT
				rayInst.accelerationStructureReference                      = v->GetAccel().accel->GetACDeviceAddress();
				rayInst.flags                                               = VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR;
				rayInst.mask                                                = 0xFF;                                                       // Only be hit if rayMask & instance.mask != 0
				rayInst.instanceShaderBindingTableRecordOffset              = v->GetHitShaderHandle();                                    // We will use the same hit group for all objects

				tlas.push_back(rayInst);

				(*desc)[index] = v->GetMeshDesc().GetBufferAddress();

				index += 1;
				return false;
			});

			return false;
		});

		rayTracingInstance->GetMeshDesc().CreateBuffer("MeshDescBuffer", VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);

		/**
		* @brief Cache this frame hit groups.
		*/
		SetHitGroupsCache(hitGroups);

		rayTracingInstance->SetHitGroups(hitGroups);

		/**
		* @brief Build TLAS.
		*/
		rayTracingInstance->BuildTLAS(
			tlas,
			VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR |
			VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR      |
			VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_COMPACTION_BIT_KHR  ,
			update
		);
	}
}