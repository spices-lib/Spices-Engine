#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"
#include "VulkanImage.h"

namespace Spiecs {

	class VulkanSwapChain : VulkanObject
	{
	public:
		VulkanSwapChain(VulkanState& vulkanState);
		virtual ~VulkanSwapChain();

		VulkanSwapChain(const VulkanSwapChain&) = delete;
		VulkanSwapChain& operator=(const VulkanSwapChain&) = delete;

		void Create();
		void Destroy();

	private:
		std::vector<VkImage> m_SwapChainImages;
		std::vector<VkImageView> m_SwapChainImageViews;
		std::vector<VkSampler> m_SwapChainImageSamplers;

		std::unique_ptr<VulkanImage> m_DepthImage;
		std::unique_ptr<VulkanImage> m_ColorImage;

		std::vector<VkFramebuffer> m_SwapChainFramebuffers;
	};
}