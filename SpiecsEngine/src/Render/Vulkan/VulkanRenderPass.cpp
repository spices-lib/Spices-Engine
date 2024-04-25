/**
* @file VulkanRenderPass.cpp.
* @brief The VulkanRenderPass Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "VulkanRenderPass.h"

namespace Spiecs {

	VulkanRenderPass::VulkanRenderPass(VulkanState& vulkanState, std::shared_ptr<VulkanDevice> vulkanDevice, std::shared_ptr<RendererResourcePool> rendererResourcePool)
		: VulkanObject(vulkanState)
		, m_VulkanDevice(vulkanDevice)
		, m_RendererResourcePool(rendererResourcePool)
	{}

	VulkanRenderPass::~VulkanRenderPass()
	{
		/**
		* @brief Destroy FrameBuffer.
		*/
		for (auto framebuffer : m_SwapChainFramebuffers) 
		{
			vkDestroyFramebuffer(m_VulkanState.m_Device, framebuffer, nullptr);
		}

		/**
		* @brief Destroy RenderPass.
		*/
		vkDestroyRenderPass(m_VulkanState.m_Device, m_RenderPass, nullptr);
	}

	void VulkanRenderPass::Build()
	{
		/**
		* @brief Instanced a VkSubpassDescription with default value.
		*/
		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

		subpass.inputAttachmentCount = (uint32_t)m_InputAttachmentRef.size();
		subpass.pInputAttachments = m_InputAttachmentRef.data();

		subpass.colorAttachmentCount = (uint32_t)m_ColorAttachmentRef.size();
		subpass.pColorAttachments = m_ColorAttachmentRef.data();

		if (isUseDpth)
		{
			subpass.pDepthStencilAttachment = &m_DepthAttachmentRef;
		}

		/**
		* @brief Instanced a VkSubpassDependency with default value.
		*/
		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;

		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

		dependency.srcAccessMask = 0;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		/**
		* @brief Instanced a VkRenderPassCreateInfo with default value.
		*/
		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(m_Attachments.size());
		renderPassInfo.pAttachments = m_Attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		/**
		* @brief CreateRenderPass.
		*/
		VK_CHECK(vkCreateRenderPass(m_VulkanState.m_Device, &renderPassInfo, nullptr, &m_RenderPass));


		for (size_t i = 0; i < MaxFrameInFlight; i++)
		{
			/**
			* @brief Add SwapChian's image to local variable.
			*/
			std::vector<VkImageView> attachments = m_AttachmentsView;

			if (IsUseSwapChianImage)
			{
				attachments.emplace(attachments.begin(), m_VulkanState.m_SwapChainImageViews[i]);
			}

			/**
			* @brief Instanced a VkFramebufferCreateInfo with default value.
			*/
			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = m_RenderPass;
			framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = m_VulkanDevice->GetSwapChainSupport().extent.width;
			framebufferInfo.height = m_VulkanDevice->GetSwapChainSupport().extent.height;
			framebufferInfo.layers = 1;

			/**
			* @brief CreateFrameBuffer.
			*/
			VK_CHECK(vkCreateFramebuffer(m_VulkanState.m_Device, &framebufferInfo, nullptr, &m_SwapChainFramebuffers[i]));
		}
	}
}