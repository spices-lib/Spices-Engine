/**
* @file SceneComposeRenderer.cpp.
* @brief The SceneComposeRenderer Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "SceneComposeRenderer.h"
#include "PreRenderer.h"
#include "Systems/SlateSystem.h"

namespace Spiecs {

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

		/***********************************Written Buffer************************************************/

		/**
		* @brief Add FinalColor Attachment.
		*/
		m_RenderPass->AddColorAttachment("SceneColor", [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		});

		/************************************************************************************************/

		/**********************************Data Buffer***************************************************/

		/**
		* @brief Add Albedo Input Attachment.
		*/
		m_RenderPass->AddInputAttachment("Diffuse", [](VkAttachmentDescription& description) {
			description.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		});

		/**
		* @brief Add Normal Input Attachment.
		*/
		m_RenderPass->AddInputAttachment("Normal", [](VkAttachmentDescription& description) {
			description.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		});

		/**
		* @brief Add Specular Input Attachment.
		*/
		m_RenderPass->AddInputAttachment("Specular", [](VkAttachmentDescription& description) {
			description.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		});

		/************************************************************************************************/

		/******************************Just transform layout not in use**********************************/

		/**
		* @brief Add Normal Input Attachment.
		*/
		m_RenderPass->AddInputAttachment("Depth", [&](VkAttachmentDescription& description) {
			description.format = VulkanSwapChain::FindDepthFormat(m_VulkanState.m_PhysicalDevice);
			description.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			description.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		});
		
		/************************************************************************************************/

		/**
		* @brief Create VkRenderPass, Resource, FrameBuffer.
		*/
		m_RenderPass->Build();
	}

	void SceneComposeRenderer::CreateDescriptorSet()
	{
		DescriptorSetBuilder{ this }
		//.AddInput(0, 0, 4, VK_SHADER_STAGE_FRAGMENT_BIT, {"Diffuse", "Normal", "Specular", "Depth"})
		.Build();
	}

	void SceneComposeRenderer::OnSlateResize()
	{
		/**
		* @brief Recreate RenderPass.
		*/
		CreateRenderPass();

	}

	void SceneComposeRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		RenderBehaverBuilder builder{ this, frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		m_Square->OnBind(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex]);
		m_Square->OnDraw(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex]);

		builder.EndRenderPass();
	}
}