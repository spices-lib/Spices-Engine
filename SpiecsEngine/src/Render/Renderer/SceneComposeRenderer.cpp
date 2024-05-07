/**
* @file SceneComposeRenderer.cpp.
* @brief The SceneComposeRenderer Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "SceneComposeRenderer.h"
#include "Systems/SlateSystem.h"

namespace Spiecs {

	namespace SceneComposeR {

		struct PushConstant
		{
			glm::vec4 gbufferSize;
			glm::vec4 windowSize;
		};
	}
	SceneComposeRenderer::SceneComposeRenderer(
		const std::string&                       rendererName         , 
		VulkanState&                             vulkanState          , 
		std::shared_ptr<VulkanDescriptorPool>    desctiptorPool       , 
		std::shared_ptr<VulkanDevice>            device               , 
		std::shared_ptr<RendererResourcePool>    rendererResourcePool
	)
		: Renderer(rendererName, vulkanState, desctiptorPool, device, rendererResourcePool)
	{
		m_Square = std::make_unique<SquarePack>();
		m_Square->OnCreatePack();
	}

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
		* @brief Add ID Input Attachment.
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
		.AddPushConstant<SceneComposeR::PushConstant>()
		.AddInput(0, 0, 5, VK_SHADER_STAGE_FRAGMENT_BIT, {"BaseColor", "Normal", "Depth", "ID"})
		//.AddTexture<Texture2D>(1, 0, 1, VK_SHADER_STAGE_FRAGMENT_BIT)
		.Build();

		/**
		* @brief create renderresource's descriptorset.
		*/
		//m_RendererResourcePool->AccessRowResource("SelectBuffer")->CreateDescriptorSet(0);
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

		ImVec2 gbufferSize = SlateSystem::GetRegister()->GetViewPort()->GetPanelSize();
		VkExtent2D windowSize = m_Device->GetSwapChainSupport().surfaceSize;
		builder.UpdatePushConstant<SceneComposeR::PushConstant>([&](auto& push) {
			push.gbufferSize = { gbufferSize.x, gbufferSize.y, 1.0f / gbufferSize.x, 1.0f / gbufferSize.y };
			push.windowSize = { windowSize.width, windowSize.height, 1.0f / windowSize.width, 1.0 / windowSize.height };
		});

		//builder.BindDescriptorSet(1, m_RendererResourcePool->AccessRowResource("SelectBuffer")->GetDescriptorSet());

		m_Square->OnBind(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex]);
		m_Square->OnDraw(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex]);

		builder.EndRenderPass();
	}
}