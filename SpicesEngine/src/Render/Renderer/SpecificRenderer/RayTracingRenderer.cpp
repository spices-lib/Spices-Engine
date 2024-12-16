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

		auto view = FrameInfo::Get().m_World->GetRegistry().view<MeshComponent>();

		AsyncTask(ThreadPoolEnum::Custom, [&, view]() {

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
			AsyncMainTask(ThreadPoolEnum::Main, [=](std::shared_ptr<VulkanRayTracing> newInstance) {

				vkQueueWaitIdle(m_VulkanState.m_GraphicQueue);
				m_RenderCache->PushToCaches(m_VulkanRayTracing);
				m_VulkanRayTracing = newInstance;

			}, rayTracingInstance);
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
	
	void RayTracingRenderer::UpdateTopLevelAS(FrameInfo& frameInfo, std::shared_ptr<VulkanRayTracing> rayTracingInstance, bool update)
	{
		SPICES_PROFILE_ZONE;

		if(!(frameInfo.m_World->GetMarker() & World::NeedUpdateTLAS)) return;
		frameInfo.m_World->ClearMarkerWithBits(World::NeedUpdateTLAS);
		
		auto view = FrameInfo::Get().m_World->GetRegistry().view<MeshComponent>();
		CreateTopLevelAS(frameInfo, view, rayTracingInstance, update);
	}

	void RayTracingRenderer::CreateRTShaderBindingTable(std::shared_ptr<VulkanRayTracing> rayTracingInstance)
	{
		SPICES_PROFILE_ZONE;

		const auto rayTracingMaterial = ResourcePool<Material>::Load<Material>("RayTracingRenderer.RayTracing.Default", "RayTracingRenderer.RayTracing.Default");
								  
		const uint32_t rayGenCount    = static_cast<uint32_t>(rayTracingMaterial->GetShaderPath("rgen").size());
		const uint32_t missCount      = static_cast<uint32_t>(rayTracingMaterial->GetShaderPath("rmiss").size());
		 
		rayTracingInstance->CreateRTShaderBindingTable(rayGenCount, missCount, m_Pipelines["RayTracingRenderer.RayTracing.Default"]->GetPipeline());
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
}