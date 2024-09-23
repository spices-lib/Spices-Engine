/**
* @file RayTracingComposeRenderer.cpp.
* @brief The RayTracingComposeRenderer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "RayTracingComposeRenderer.h"
#include "Systems/SlateSystem.h"

namespace Spices {

	RayTracingComposeRenderer::RayTracingComposeRenderer(
		const std::string&                           rendererName        ,
		VulkanState&                                 vulkanState         ,
		const std::shared_ptr<VulkanDescriptorPool>& descriptorPool      ,
		const std::shared_ptr<VulkanDevice>&         device              ,
		const std::shared_ptr<RendererResourcePool>& rendererResourcePool,
		const std::shared_ptr<VulkanCmdThreadPool>&  cmdThreadPool
	)
		: Renderer(rendererName, vulkanState, descriptorPool, device, rendererResourcePool, cmdThreadPool)
	{}

	void RayTracingComposeRenderer::CreateRendererPass()
	{
		SPICES_PROFILE_ZONE;

		RendererPassBuilder{ "RayTracingCompose", this }
		.AddSubPass("RayTracingCompose")
		.AddColorAttachment("SceneColor", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout     = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp            = VK_ATTACHMENT_LOAD_OP_CLEAR;
		})
		.AddColorAttachment("ETMID", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout     = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp            = VK_ATTACHMENT_LOAD_OP_CLEAR;
			description.format            = VK_FORMAT_R32G32B32A32_UINT;
		})
		.EndSubPass()
		.Build();
	}

	void RayTracingComposeRenderer::CreateDescriptorSet()
	{
		SPICES_PROFILE_ZONE;

		DescriptorSetBuilder{ "RayTracingCompose", this }
		.AddStorageTexture(2, 0, VK_SHADER_STAGE_FRAGMENT_BIT, { "RayImage" })
		.AddStorageTexture(2, 1, VK_SHADER_STAGE_FRAGMENT_BIT, { "RayEntityID", "RayTriangleID" })
		.Build();
	}

	std::shared_ptr<VulkanPipeline> RayTracingComposeRenderer::CreatePipeline(
		std::shared_ptr<Material>        material ,
		VkPipelineLayout&                layout   ,
		std::shared_ptr<RendererSubPass> subPass
	)
	{
		SPICES_PROFILE_ZONE;

		PipelineConfigInfo pipelineConfig{};
		VulkanPipeline::DefaultPipelineConfigInfo(pipelineConfig);

		pipelineConfig.bindingDescriptions = {};
		pipelineConfig.attributeDescriptions = {};

		pipelineConfig.renderPass = m_Pass->Get();
		pipelineConfig.subpass = subPass->GetIndex();
		pipelineConfig.pipelineLayout = layout;
		pipelineConfig.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
		pipelineConfig.colorBlendInfo.attachmentCount = static_cast<uint32_t>(subPass->GetColorBlend().size());
		pipelineConfig.colorBlendInfo.pAttachments = subPass->GetColorBlend().data();

		return std::make_shared<VulkanPipeline>(
			m_VulkanState,
			material->GetName(),
			material->GetShaderPath(),
			pipelineConfig
		);
	}

	void RayTracingComposeRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		if(frameInfo.m_RendererType != RendererType::PathTracing) return;
		
		RenderBehaveBuilder builder{ this, frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.BeginRenderPass();

		builder.SetViewPort();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "RayTracingCompose" }));

		builder.BindPipeline("RayTracingComposeRenderer.RayTracingCompose.Default");

		builder.DrawFullScreenTriangle();

		builder.EndRenderPass();
	}
}