/**
* @file BasePassRenderer.cpp.
* @brief The BasePassRenderer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "BasePassRenderer.h"

namespace Spices {

	/**
	* @brief Use DGC or not.
	*/
	constexpr bool m_IsUseDGC = false;

	void BasePassRenderer::CreateRendererPass()
	{
		SPICES_PROFILE_ZONE;

		RendererPassBuilder{ "BassPass", this }
		.AddSubPass("Mesh")
		.AddColorAttachment("Albedo", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp                = VK_ATTACHMENT_LOAD_OP_CLEAR;
			description.format                = VK_FORMAT_R16G16B16A16_SFLOAT;
		})
		.AddColorAttachment("Normal", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp                = VK_ATTACHMENT_LOAD_OP_CLEAR;
			description.format                = VK_FORMAT_R16G16B16A16_SFLOAT;
		})
		.AddColorAttachment("Roughness", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp                = VK_ATTACHMENT_LOAD_OP_CLEAR;
			description.format                = VK_FORMAT_R16G16B16A16_SFLOAT;
		})
		.AddColorAttachment("Metallic", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp                = VK_ATTACHMENT_LOAD_OP_CLEAR;
			description.format                = VK_FORMAT_R16G16B16A16_SFLOAT;
		})
		.AddColorAttachment("Position", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp                = VK_ATTACHMENT_LOAD_OP_CLEAR;
			description.format                = VK_FORMAT_R32G32B32A32_SFLOAT;
		})
		.AddColorAttachment("EntityID", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp                = VK_ATTACHMENT_LOAD_OP_CLEAR;
			description.format                = VK_FORMAT_R32_SFLOAT;
		})
		.AddColorAttachment("TriangleID", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp                = VK_ATTACHMENT_LOAD_OP_CLEAR;
			description.finalLayout           = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		})
		.AddColorAttachment("MeshletID", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp                = VK_ATTACHMENT_LOAD_OP_CLEAR;
			description.finalLayout           = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		})
		.AddDepthAttachment("Depth", TextureType::Texture2D, [](VkAttachmentDescription& description) {
			description.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;   /* @attention It seams that layout transform is not work? */
			description.loadOp                = VK_ATTACHMENT_LOAD_OP_CLEAR;
		})
		.AddSelfDependency(VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV,VK_ACCESS_INDIRECT_COMMAND_READ_BIT,VK_PIPELINE_STAGE_COMMAND_PREPROCESS_BIT_NV,VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT)
		.EndSubPass()
		.AddSubPass("SkyBox", Querier::None)
		.AddColorAttachment("Albedo", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.format                = VK_FORMAT_R16G16B16A16_SFLOAT;
		})
		.AddColorAttachment("Position", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.format                = VK_FORMAT_R32G32B32A32_SFLOAT;
		})
		.AddColorAttachment("EntityID", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.format                = VK_FORMAT_R32_SFLOAT;
		})
		.AddColorAttachment("TriangleID", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
		})
		.AddColorAttachment("MeshletID", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
		})
		.AddDepthAttachment("Depth", TextureType::Texture2D, [](VkAttachmentDescription& description) {})
		.EndSubPass()
		.Build();
	}

	void BasePassRenderer::CreateDescriptorSet()
	{
		SPICES_PROFILE_ZONE;

		DescriptorSetBuilder{ "Mesh", this }
		.AddPushConstant(sizeof(uint64_t))
		.Build();

		DescriptorSetBuilder{ "SkyBox", this }
		.AddPushConstant(sizeof(uint64_t))
		.Build();
	}
	
	void BasePassRenderer::CreateDeviceGeneratedCommandsLayout()
	{
		SPICES_PROFILE_ZONE;

		DGCLayoutBuilder{ "Mesh", this }
		.AddShaderGroupInput()
		.AddPushConstantInput()
		.AddDrawMeshTaskInput()
		.Build();
	}

	void BasePassRenderer::OnMeshAddedWorld()
	{
		Renderer::OnMeshAddedWorld();

		const auto view = GetEntityWithComponent<MeshComponent>(FrameInfo::Get().m_World.get());
		
		//AsyncTask(ThreadPoolEnum::Custom, [=]() {

			SPICES_PROFILE_ZONEN("RayTracingRenderer::OnMeshAddedWorld");

			const auto dgcInstance = FillIndirectRenderData<MeshComponent>("Mesh", view);

			//AsyncMainTask(ThreadPoolEnum::Main, [=]() {

				vkQueueWaitIdle(m_VulkanState.m_GraphicQueue);
				m_DGCData["Mesh"] = dgcInstance;
				m_View = view;

			//});
		//});
	}

	void BasePassRenderer::CreatePipeline(
		std::shared_ptr<Material>        material ,
		VkPipelineLayout&                layout   ,
		std::shared_ptr<RendererSubPass> subPass
	)
	{
		SPICES_PROFILE_ZONE;

		PipelineBuilder{ subPass, material, this }
		.SetDefault()
		.SetRenderPass()
		.SetSubPassIndex()
		.SetPipelineLayout(layout)
		.SetCullMode(VK_CULL_MODE_NONE)
		.SetColorAttachments()
		.BuildMesh();
	}

	void BasePassRenderer::CreateDeviceGeneratedCommandsPipeline(
		const std::string&               pipelineName ,
		const std::string&               materialName ,
		VkPipelineLayout&                layout       ,
		std::shared_ptr<RendererSubPass> subPass      ,
		VulkanDeviceGeneratedCommandsNV* indirectPtr
	)
	{
		SPICES_PROFILE_ZONE;

		PipelineBuilder{ subPass, nullptr, this }
		.SetDefault()
		.SetRenderPass()
		.SetSubPassIndex()
		.SetPipelineLayout(layout)
		.SetCullMode(VK_CULL_MODE_NONE)
		.SetColorAttachments()
		.BuildDeviceGeneratedCommand(pipelineName, materialName, indirectPtr);
	}

	void BasePassRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		if(frameInfo.m_RendererType != RendererType::Rasterization) return;
		
		RenderBehaveBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_ImageIndex };
		
		builder.BeginRenderPassAsync();

		if constexpr (m_IsUseDGC)
		{
			builder.Await([&](const VkCommandBuffer& cmdBuffer) {

				builder.SetViewPort(cmdBuffer);
			
				builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"), cmdBuffer);
			
				builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "Mesh" }), cmdBuffer);

				builder.RunDGC(cmdBuffer);

			});
		}
		else
		{
			const size_t nThreads = ThreadModel::Get()->GetRHIThreadPool()->GetThreadsCount();
			const size_t nTask    = m_View ? (m_View->size() > 40 ? nThreads : 1) : 0;
			const size_t nCount   = m_View ? m_View->size() / nTask : 0;

			std::vector<std::future<VkCommandBuffer>> futureCmdBuffers;
			for(size_t i = 0; i < nTask; i++)
			{
				futureCmdBuffers.push_back(

				builder.Async([&, i](const VkCommandBuffer& cmdBuffer) {

					builder.SetViewPort(cmdBuffer);

					builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"), cmdBuffer);

					builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "Mesh" }), cmdBuffer);

					IterWorldCompWithRange<MeshComponent>(frameInfo, *m_View, i * nCount, std::min((i + 1) * nCount, m_View->size() - 1), [&](int entityId, TransformComponent& transComp, MeshComponent& meshComp) {

						meshComp.GetMesh()->DrawMeshTasks(cmdBuffer, [&](const uint32_t& meshpackId, const auto& meshPack) {

							builder.BindPipeline(meshPack->GetMaterial()->GetName(), cmdBuffer);

							builder.UpdatePushConstant<uint64_t>([&](auto& push) {
								push = meshPack->GetMeshDesc().GetBufferAddress();
							}, cmdBuffer);
						});
					});
				})
				);
			}

			if (nTask > 0)
			{
				builder.Wait(futureCmdBuffers);
			}
		}

		builder.BeginNextSubPass("SkyBox");

		builder.SetViewPort();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "SkyBox" }));

		IterWorldCompWithBreak<SkyBoxComponent>(frameInfo, [&](int entityId, TransformComponent& transComp, SkyBoxComponent& skyboxComp) {

			skyboxComp.GetMesh()->DrawMeshTasks(m_VulkanState.m_GraphicCommandBuffer[frameInfo.m_FrameIndex], [&](const uint32_t& meshPackId, const auto& meshPack) {

				builder.BindPipeline(meshPack->GetMaterial()->GetName());

				builder.UpdatePushConstant<uint64_t>([&](auto& push) {
					push = meshPack->GetMeshDesc().GetBufferAddress();
				});
			});

			return true;
		});

		builder.EndRenderPass();
	}
}