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


		VulkanRenderPass(
			VulkanState& vulkanState,
			const std::string& passName,
			std::shared_ptr<VulkanDevice> vulkanDevice,
			VkRenderPassCreateInfo& createInfo,
			std::vector<VkImageView>& imageViews,
			bool isUseSwapChianImage
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