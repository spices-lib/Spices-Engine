/**
* @file VulkanRenderPass.cpp.
* @brief The VulkanRenderPass Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "VulkanRenderPass.h"
#include "Slate/Imgui/ViewPort/ImguiViewport.h"

namespace Spiecs {

	VulkanRenderPass::VulkanRenderPass(
		VulkanState & vulkanState, 
		std::shared_ptr<VulkanDevice> vulkanDevice,  
		VkRenderPassCreateInfo & createInfo, 
		std::vector<VkImageView>& imageViews,
		bool isUseSwapChianImage
	)
		: VulkanObject(vulkanState)
		, m_VulkanDevice(vulkanDevice)
	{
		/**
		* @brief CreateRenderPass.
		*/
		VK_CHECK(vkCreateRenderPass(m_VulkanState.m_Device, &createInfo, nullptr, &m_RenderPass));

		for (size_t i = 0; i < MaxFrameInFlight; i++)
		{
			/**
			* @brief Add SwapChian's image to local variable.
			*/

			if (isUseSwapChianImage)
			{
				imageViews.emplace(imageViews.begin(), m_VulkanState.m_SwapChainImageViews[i]);
			}
			
			/**
			* @brief Instanced a VkFramebufferCreateInfo with default value.
			*/
			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = m_RenderPass;
			framebufferInfo.attachmentCount = static_cast<uint32_t>(imageViews.size());
			framebufferInfo.pAttachments = imageViews.data();

			if (isUseSwapChianImage || !SlateSystem::GetRegister())
			{
				framebufferInfo.width = m_VulkanDevice->GetSwapChainSupport().surfaceSize.width;
				framebufferInfo.height = m_VulkanDevice->GetSwapChainSupport().surfaceSize.height;
			}
			else
			{
				framebufferInfo.width = static_cast<uint32_t>(SlateSystem::GetRegister()->GetViewPort()->GetPanelSize().x);
				framebufferInfo.height = static_cast<uint32_t>(SlateSystem::GetRegister()->GetViewPort()->GetPanelSize().y);
			}

			framebufferInfo.layers = 1;

			/**
			* @brief CreateFrameBuffer.
			*/
			VK_CHECK(vkCreateFramebuffer(m_VulkanState.m_Device, &framebufferInfo, nullptr, &m_SwapChainFramebuffers[i]));
		}
	}

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
}