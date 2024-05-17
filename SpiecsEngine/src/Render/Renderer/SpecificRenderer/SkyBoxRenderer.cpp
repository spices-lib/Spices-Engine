/**
* @file SkyBoxRenderer.cpp.
* @brief The SkyBoxRenderer Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "SkyBoxRenderer.h"
#include "PreRenderer.h"

namespace Spiecs {

	void SkyBoxRenderer::CreateRenderPass()
	{
		/**
		* @brief Declear an empty VulkanRenderPass Object.
		*/
		m_RenderPass = std::make_unique<VulkanRenderPass>(m_VulkanState, m_Device, m_RendererResourcePool);

		/**
		* @brief Add Albedo Attachment.
		*/
		m_RenderPass->AddColorAttachment("Diffuse", [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		});

		/**
		* @brief Add ID Attachment.
		*/
		m_RenderPass->AddColorAttachment("ID", [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			description.format = VK_FORMAT_R32_SFLOAT;
		});

		/**
		* @brief Add Depth Attachment.
		*/
		m_RenderPass->AddDepthAttachment([](VkAttachmentDescription& description) {
			description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		});

		/**
		* @brief Create VkRenderPass, Resource, FrameBuffer.
		*/
		m_RenderPass->Build();
	}

	void SkyBoxRenderer::CreateDescriptorSet()
	{
		DescriptorSetBuilder{ this }
		.AddPushConstant<PreR::PushConstant>()
		.Build();
	}

	std::shared_ptr<VulkanPipeline> SkyBoxRenderer::CreatePipeline(
		std::shared_ptr<Material> material    , 
		VkRenderPass&             renderPass  , 
		VkPipelineLayout&         layout
	)
	{
		PipelineConfigInfo pipelineConfig{};
		VulkanPipeline::DefaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = layout;
		pipelineConfig.rasterizationInfo.cullMode = VK_CULL_MODE_FRONT_BIT;
		pipelineConfig.colorBlendInfo.attachmentCount = (uint32_t)m_RenderPass->GetColorBlend().size();
		pipelineConfig.colorBlendInfo.pAttachments = m_RenderPass->GetColorBlend().data();
		return std::make_shared<VulkanPipeline>(
			m_VulkanState,
			GetSahderPath(material->GetShaderPath("vertShader"), "vert"),
			GetSahderPath(material->GetShaderPath("fragShader"), "frag"),
			pipelineConfig
		);
	}

	void SkyBoxRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		RenderBehaverBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.BeginRenderPass();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		IterWorldComp<SkyBoxComponent>(frameInfo, [&](int entityId, TransformComponent& transComp, SkyBoxComponent& skyboxComp) {
			const glm::mat4& modelMatrix = transComp.GetModelMatrix();

			skyboxComp.GetMesh()->Draw(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex], [&](uint32_t meshpackId, auto material) {
				builder.BindPipeline(material->GetName());

				builder.UpdatePushConstant<PreR::PushConstant>([&](auto& push) {
					push.model = modelMatrix;
					push.entityID = entityId;
				});

				builder.BindDescriptorSet(material->GetMaterialDescriptorSet(), material->GetName());
			});
			
			return false;
		});

		builder.EndRenderPass();
	}
}