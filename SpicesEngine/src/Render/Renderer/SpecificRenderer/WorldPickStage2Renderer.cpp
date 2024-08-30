/**
* @file WorldPickRenderer.cpp.
* @brief The WorldPickRenderer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "WorldPickStage2Renderer.h"
#include "PreRenderer.h"
#include "Systems/SlateSystem.h"

namespace Spices {

	WorldPickStage2Renderer::WorldPickStage2Renderer(
		const std::string&                           rendererName           , 
		VulkanState&                                 vulkanState            ,
		const std::shared_ptr<VulkanDescriptorPool>& descriptorPool         ,
		const std::shared_ptr<VulkanDevice>&         device                 ,
		const std::shared_ptr<RendererResourcePool>& rendererResourcePool   ,
		const std::shared_ptr<VulkanCmdThreadPool>&  cmdThreadPool
	)
		: Renderer(rendererName, vulkanState, descriptorPool, device, rendererResourcePool, cmdThreadPool)
	{}

	void WorldPickStage2Renderer::CreateRendererPass()
	{
		SPICES_PROFILE_ZONE;

		RendererPassBuilder{ "WorldPickStage2", this }
		.AddSubPass("WorldPickStage2")
		.AddColorAttachment("SceneColor", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.finalLayout                          = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		})
		.EndSubPass()
		.Build();

		/**
		* @brief create renderresource's descriptorset.
		*/
		m_RendererResourcePool->AccessRowResource("SelectBuffer")->CreateDescriptorSet(0);
	}

	void WorldPickStage2Renderer::CreateDescriptorSet()
	{
		SPICES_PROFILE_ZONE;

		DescriptorSetBuilder{ "WorldPickStage2", this }
		.AddAttachmentTexture(2, 0, VK_SHADER_STAGE_FRAGMENT_BIT, {"SelectBuffer"})
		.Build();
	}

	std::shared_ptr<VulkanPipeline> WorldPickStage2Renderer::CreatePipeline(
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

	void WorldPickStage2Renderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		RenderBehaveBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.BeginRenderPass();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "WorldPickStage2" }));

		builder.BindPipeline("WorldPickStage2Renderer.WorldPickStage2.Default");

		builder.DrawFullScreenTriangle();

		builder.EndRenderPass();
	}
}