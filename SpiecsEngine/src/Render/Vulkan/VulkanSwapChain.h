#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"
#include "VulkanImage.h"
#include "VulkanDevice.h"

namespace Spiecs {

	class VulkanSwapChain : VulkanObject
	{
	public:
		VulkanSwapChain(VulkanState& vulkanState, std::shared_ptr<VulkanDevice> vulkanDevice);
		virtual ~VulkanSwapChain();

		static VkFormat FindDepthFormat(const VkPhysicalDevice& physicalDevice);
		static VkFormat findSupportedFormat(const VkPhysicalDevice& physicalDevice, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

		VkFramebuffer& GetFramebuffer(uint32_t index) { return m_SwapChainFramebuffers[index]; };

		void Create();
		void Destroy();

	private:
		void CreateSyncObjects();
		void DestroySyncObjects();
	private:
		std::shared_ptr<VulkanDevice> m_VulkanDevice;

		std::array<VkImage, MaxFrameInFlight> m_SwapChainImages;
		std::array<VkImageView, MaxFrameInFlight> m_SwapChainImageViews;
		std::array<VkSampler, MaxFrameInFlight> m_SwapChainImageSamplers;

		std::unique_ptr<VulkanImage> m_ColorImage;
		std::unique_ptr<VulkanImage> m_DepthImage;
		// m_SwapChainImages[i]


		std::array<VkFramebuffer, MaxFrameInFlight> m_SwapChainFramebuffers;
	};
}