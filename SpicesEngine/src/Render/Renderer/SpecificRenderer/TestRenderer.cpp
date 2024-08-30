#include "Pchheader.h"
#include "TestRenderer.h"
#include "PreRenderer.h"
#include "Systems/SlateSystem.h"

namespace Spices {

	TestRenderer::TestRenderer(
		const std::string&                           rendererName   ,
		VulkanState&                                 vulkanState    ,
		const std::shared_ptr<VulkanDescriptorPool>& desctiptorPool ,
		const std::shared_ptr<VulkanDevice>&         device         ,
		const std::shared_ptr<RendererResourcePool>& rendererResourcePool,
		const std::shared_ptr<VulkanCmdThreadPool>&  cmdThreadPool
	)
		: Renderer(rendererName, vulkanState, desctiptorPool, device, rendererResourcePool, cmdThreadPool)
	{}

	void TestRenderer::CreateRendererPass()
	{
		SPICES_PROFILE_ZONE;

		RendererPassBuilder{ "TestPass", this }
		.AddSubPass("Test")
		.AddColorAttachment("TestCube", TextureType::Texture2DCube, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout                             = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp                                    = VK_ATTACHMENT_LOAD_OP_CLEAR;
			description.finalLayout                               = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		})
		.EndSubPass()
		.Build();
	}

	void TestRenderer::CreateDescriptorSet()
	{
		SPICES_PROFILE_ZONE;

		DescriptorSetBuilder{ "Test", this }
		.Build();
	}

	std::shared_ptr<VulkanPipeline> TestRenderer::CreatePipeline(
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

	void TestRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		RenderBehaveBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.BeginRenderPass();

		builder.BindPipeline("TestRenderer.Test.Default");

		builder.DrawFullScreenTriangle();

		builder.EndRenderPass();
	}
}