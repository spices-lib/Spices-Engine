/**
* @file ViewportGridRenderer.cpp.
* @brief The ViewportGridRenderer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ViewportGridRenderer.h"

namespace Spices {

	void ViewportGridRenderer::CreateRendererPass()
	{
		SPICES_PROFILE_ZONE;

		RendererPassBuilder{ "ViewportGrid", this }
		.AddSubPass("ViewportGrid")
		.AddColorAttachment("SceneColor", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			isEnableBlend                            = true;
		})
		.EndSubPass()
		.Build();
	}

	void ViewportGridRenderer::CreateDescriptorSet()
	{
		SPICES_PROFILE_ZONE;

		DescriptorSetBuilder{ "ViewportGrid", this }
		.Build();
	}

	std::shared_ptr<VulkanPipeline> ViewportGridRenderer::CreatePipeline(
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

	void ViewportGridRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		RenderBehaveBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.BeginRenderPass();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "ViewportGrid" }));
		
		builder.BindPipeline("ViewportGridRenderer.ViewportGrid.Default");

		builder.DrawFullScreenTriangle();

		builder.EndRenderPass();
	}
}