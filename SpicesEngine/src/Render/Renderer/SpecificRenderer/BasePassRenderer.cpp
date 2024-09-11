/**
* @file BasePassRenderer.cpp.
* @brief The BasePassRenderer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "BasePassRenderer.h"

namespace Spices {

	void BasePassRenderer::CreateRendererPass()
	{
		SPICES_PROFILE_ZONE;

		RendererPassBuilder{ "BassPass", this }
		.AddSubPass("Mesh")
		.AddColorAttachment("Albedo", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp                = VK_ATTACHMENT_LOAD_OP_CLEAR;
		})
		.AddColorAttachment("Normal", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp                = VK_ATTACHMENT_LOAD_OP_CLEAR;
		})
		.AddColorAttachment("Roughness", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp                = VK_ATTACHMENT_LOAD_OP_CLEAR;
		})
		.AddColorAttachment("Metallic", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp                = VK_ATTACHMENT_LOAD_OP_CLEAR;
		})
		.AddColorAttachment("Position", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp                = VK_ATTACHMENT_LOAD_OP_CLEAR;
			description.format                = VK_FORMAT_R32G32B32A32_SFLOAT;
		})
		.AddColorAttachment("ID", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
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
		.AddSubPass("SkyBox")
		.AddColorAttachment("Albedo", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {})
		.AddColorAttachment("Position", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.format                = VK_FORMAT_R32G32B32A32_SFLOAT;
		})
		.AddColorAttachment("ID", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
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
		SPICES_PROFILE_ZONE;

		FillIndirectRenderData<MeshComponent>("Mesh");	
	}

	std::shared_ptr<VulkanPipeline> BasePassRenderer::CreatePipeline(
		std::shared_ptr<Material>        material ,
		VkPipelineLayout&                layout   ,
		std::shared_ptr<RendererSubPass> subPass
	)
	{
		SPICES_PROFILE_ZONE;

		PipelineConfigInfo pipelineConfig{};
		VulkanPipeline::DefaultPipelineConfigInfo(pipelineConfig);
		//pipelineConfig.rasterizationInfo.polygonMode = VK_POLYGON_MODE_LINE;
		pipelineConfig.renderPass                      = m_Pass->Get();
		pipelineConfig.subpass                         = subPass->GetIndex();
		pipelineConfig.pipelineLayout                  = layout;
		pipelineConfig.rasterizationInfo.cullMode      = VK_CULL_MODE_NONE;
		pipelineConfig.colorBlendInfo.attachmentCount  = static_cast<uint32_t>(subPass->GetColorBlend().size());
		pipelineConfig.colorBlendInfo.pAttachments     = subPass->GetColorBlend().data();

		return std::make_shared<VulkanMeshPipeline>(
			m_VulkanState,
			material->GetName(),
			material->GetShaderPath(),
			pipelineConfig
		);
	}

	std::shared_ptr<VulkanPipeline> BasePassRenderer::CreateDGCPipeline(
		const std::string&               pipelineName ,
		const std::string&               materialName ,
		VkPipelineLayout&                layout       ,
		std::shared_ptr<RendererSubPass> subPass
	)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Get Dafault PipelineConfigInfo.
		*/
		PipelineConfigInfo pipelineConfig{};
		VulkanPipeline::DefaultPipelineConfigInfo(pipelineConfig);

		/**
		* @brief Fill in with configurable data.
		*/
		pipelineConfig.renderPass                     = m_Pass->Get();
		pipelineConfig.subpass                        = subPass->GetIndex();
		pipelineConfig.pipelineLayout                 = layout;
		pipelineConfig.colorBlendInfo.attachmentCount = static_cast<uint32_t>(subPass->GetColorBlend().size());
		pipelineConfig.colorBlendInfo.pAttachments    = subPass->GetColorBlend().data();

		/**
		* @brief Create VulkanPipeline.
		*/
		return std::make_shared<VulkanIndirectMeshPipelineNV>(
			m_VulkanState,
			pipelineName,
			materialName,
			m_PipelinesRef[subPass->GetName()],
			pipelineConfig
		);

		m_PipelinesRef.clear();
	}

	void BasePassRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		if(frameInfo.m_RendererType != RendererType::Rasterization) return;
		
		RenderBehaveBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };
		
		builder.BeginRenderPassAsync();

		builder.Async([&](VkCommandBuffer& cmdBuffer) {

			builder.SetViewPort(cmdBuffer);

			builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"), cmdBuffer);

			builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "Mesh" }), cmdBuffer);

			builder.RunDGC(cmdBuffer);
		});

		builder.BeginNextSubPass("SkyBox");

		builder.SetViewPort();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "SkyBox" }));

		IterWorldCompWithBreak<SkyBoxComponent>(frameInfo, [&](int entityId, TransformComponent& transComp, SkyBoxComponent& skyboxComp) {

			skyboxComp.GetMesh()->DrawMeshTasks(m_VulkanState.m_GraphicCommandBuffer[frameInfo.m_FrameIndex], [&](const uint32_t& meshpackId, const auto& meshPack) {

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