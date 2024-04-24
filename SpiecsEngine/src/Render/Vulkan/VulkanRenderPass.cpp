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

		/**
		* @brief Fill in pipelineBindPoint.
		*/
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

		/**
		* @brief Fill in colorAttachmentCount.
		*/
		subpass.colorAttachmentCount = (uint32_t)m_ColorAttachmentRef.size();

		/**
		* @brief Fill in pColorAttachments.
		*/
		subpass.pColorAttachments = m_ColorAttachmentRef.data();

		/**
		* @brief Fill in pDepthStencilAttachment.
		*/
		subpass.pDepthStencilAttachment = &m_DepthAttachmentRef;

		/**
		* @brief Instanced a VkSubpassDependency with default value.
		*/
		VkSubpassDependency dependency{};

		/**
		* @brief Fill in srcSubpass.
		*/
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;

		/**
		* @brief Fill in dstSubpass.
		*/
		dependency.dstSubpass = 0;

		/**
		* @brief Fill in srcStageMask.
		*/
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

		/**
		* @brief Fill in srcAccessMask.
		*/
		dependency.srcAccessMask = 0;

		/**
		* @brief Fill in dstStageMask.
		*/
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

		/**
		* @brief Fill in dstAccessMask.
		*/
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		/**
		* @brief Instanced a VkRenderPassCreateInfo with default value.
		*/
		VkRenderPassCreateInfo renderPassInfo{};

		/**
		* @brief Fill in sType.
		*/
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

		/**
		* @brief Fill in attachmentCount.
		*/
		renderPassInfo.attachmentCount = static_cast<uint32_t>(m_Attachments.size());

		/**
		* @brief Fill in pAttachments.
		*/
		renderPassInfo.pAttachments = m_Attachments.data();

		/**
		* @brief Fill in subpassCount.
		*/
		renderPassInfo.subpassCount = 1;

		/**
		* @brief Fill in pSubpasses.
		*/
		renderPassInfo.pSubpasses = &subpass;

		/**
		* @brief Fill in dependencyCount.
		*/
		renderPassInfo.dependencyCount = 1;

		/**
		* @brief Fill in pDependencies.
		*/
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

			/**
			* @brief Fill in sType.
			*/
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;

			/**
			* @brief Fill in renderPass.
			*/
			framebufferInfo.renderPass = m_RenderPass;

			/**
			* @brief Fill in attachmentCount.
			*/
			framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());

			/**
			* @brief Fill in pAttachments.
			*/
			framebufferInfo.pAttachments = attachments.data();

			/**
			* @brief Fill in width.
			*/
			framebufferInfo.width = m_VulkanDevice->GetSwapChainSupport().extent.width;

			/**
			* @brief Fill in height.
			*/
			framebufferInfo.height = m_VulkanDevice->GetSwapChainSupport().extent.height;

			/**
			* @brief Fill in layers.
			*/
			framebufferInfo.layers = 1;

			/**
			* @brief CreateFrameBuffer.
			*/
			VK_CHECK(vkCreateFramebuffer(m_VulkanState.m_Device, &framebufferInfo, nullptr, &m_SwapChainFramebuffers[i]));
		}
	}
}