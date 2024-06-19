/**
* @file VulkanRenderPass.h.
* @brief The VulkanRenderPass Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include "Render/RendererResource/RendererResourcePool.h"
#include "Systems/SlateSystem.h"

namespace Spiecs {

	/**
	* @brief VulkanRenderPass Class.
	* This class defines the VulkanRenderPass behaver.
	* This class is just a wapper of VkRenderPass, VkFramebuffer.
	*/
	class VulkanRenderPass : public VulkanObject
	{
	public:

		/**
		* @brief Constructor Function.
		* Create VkRenderPass and VkFramebuffer.
		* @param[in] vulkanState The global VulkanState.
		* @param[in] passName The RenderPass's name.
		* @param[in] vulkanDevice The shared pointer of VulkanDevice.
		* @param[in] createInfo The information helps to create a VkRenderPass.
		* @param[in] imageViews All attachments except swapchain image view that in used.
		* @param[in] layers The max layers of all attachments.
		* @param[in] isUseSwapChianImage True if need apped a swpachain image view.
		*/
		VulkanRenderPass(
			VulkanState&                  vulkanState           ,
			const std::string&            passName              ,
			std::shared_ptr<VulkanDevice> vulkanDevice          ,
			VkRenderPassCreateInfo&       createInfo            ,
			std::vector<VkImageView>&     imageViews            ,
			uint32_t                      layers                ,
			bool                          isUseSwapChianImage
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanRenderPass();

		/**
		* @brief Get VkRenderPass.
		* @return Returns the VkRenderPass.
		*/
		inline VkRenderPass& Get() { return m_RenderPass; };

		/**
		* @brief Get Framebuffer by index.
		* @param[in] index Frameindex in FrameInfo.
		* @return Returns the indexed Framebuffer.
		*/
		inline VkFramebuffer& GetFramebuffer(uint32_t index) { return m_SwapChainFramebuffers[index]; };

	private:

		/**
		* @brief The shared pointer of VulkanDevice.
		*/
		std::shared_ptr<VulkanDevice> m_VulkanDevice;

		/**
		* @brief The RenderPass this class mainly manage.
		*/
		VkRenderPass m_RenderPass{};

		/**
		* @brief The FrameBuffers.
		*/
		std::array<VkFramebuffer, MaxFrameInFlight> m_SwapChainFramebuffers{};
	};
}