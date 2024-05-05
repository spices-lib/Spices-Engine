/**
* @file SceneComposeRenderer.cpp.
* @brief The SceneComposeRenderer Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "SceneComposeRenderer.h"

namespace Spiecs {

	void SceneComposeRenderer::CreateRenderPass()
	{
		m_RenderPass = std::make_unique<VulkanRenderPass>(m_VulkanState, m_Device, m_RendererResourcePool);

		/**
		* @brief Add FinalColor Attachment.
		*/
		m_RenderPass->AddColorAttachment("FinalColor", [](VkAttachmentDescription& description) {
			description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			description.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			description.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		});

		/**
		* @brief Add BaseColor Input Attachment.
		*/
		m_RenderPass->AddInputAttachment("BaseColor", [](VkAttachmentDescription& description) {
			description.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		});

		/**
		* @brief Add Normal Input Attachment.
		*/
		m_RenderPass->AddInputAttachment("Normal", [](VkAttachmentDescription& description) {
			description.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		});

		/**
		* @brief Add Normal Input Attachment.
		*/
		m_RenderPass->AddInputAttachment("Depth", [&](VkAttachmentDescription& description) {
			description.format = VulkanSwapChain::FindDepthFormat(m_VulkanState.m_PhysicalDevice);
			description.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			description.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		});

		/**
		* @brief Add Normal Input Attachment.
		*/
		m_RenderPass->AddInputAttachment("ID", [](VkAttachmentDescription& description) {
			description.format = VK_FORMAT_R32_SFLOAT;
			description.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		});
		
		/**
		* @brief Create VkRenderPass, Resource, FrameBuffer.
		*/
		m_RenderPass->Build();
	}

	void SceneComposeRenderer::CreatePipelineLayoutAndDescriptor()
	{
		PipelineLayoutBuilder{ this }
		.AddInput(0, 0, 3, VK_SHADER_STAGE_FRAGMENT_BIT, {"BaseColor", "Normal", "ID" })
		.Build();
	}

	void SceneComposeRenderer::CreatePipeline(VkRenderPass renderPass)
	{
		PipelineConfigInfo pipelineConfig{};
		VulkanPipeline::DefaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = m_PipelineLayout;
		pipelineConfig.colorBlendInfo.attachmentCount = (uint32_t)m_RenderPass->GetColorBlend().size();
		pipelineConfig.colorBlendInfo.pAttachments = m_RenderPass->GetColorBlend().data();
		m_VulkanPipeline = std::make_unique<VulkanPipeline>(
			m_VulkanState,
			GetSahderPath("vert"),
			GetSahderPath("frag"),
			pipelineConfig
		);
	}

	void SceneComposeRenderer::OnSlateResize()
	{
		/**
		* @brief Recreate RenderPass.
		*/
		CreateRenderPass();

		/**
		* @brief Free all descriptor this renderer holded.
		*/
		FreeResource();

		/**
		* @brief Destroy PipelineLayout.
		*/
		vkDestroyPipelineLayout(m_VulkanState.m_Device, m_PipelineLayout, nullptr);

		/**
		* @brief Recreate PipelineLayout.
		*/
		CreatePipelineLayoutAndDescriptor();
	}

	void SceneComposeRenderer::Render(FrameInfo& frameInfo)
	{
		RenderBehaverBuilder builder{ this, frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		vkCmdDraw(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex], 3, 1, 0, 0);

		builder.EndRenderPass();
	}
}