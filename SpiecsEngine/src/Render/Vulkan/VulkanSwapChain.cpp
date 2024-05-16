#include "Pchheader.h"
#include "VulkanSwapChain.h"

namespace Spiecs {

	VulkanSwapChain::VulkanSwapChain(VulkanState& vulkanState, std::shared_ptr<VulkanDevice> vulkanDevice)
		: VulkanObject(vulkanState)
		, m_VulkanDevice(vulkanDevice)
	{
		Create();
		CreateSyncObjects();
	}

	VulkanSwapChain::~VulkanSwapChain()
	{
		DestroySyncObjects();
		Destroy();
	}

	VkFormat VulkanSwapChain::FindDepthFormat(const VkPhysicalDevice& physicalDevice)
	{
		return findSupportedFormat(
			physicalDevice, 
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
		);
	}

	VkFormat VulkanSwapChain::findSupportedFormat(const VkPhysicalDevice& physicalDevice, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
	{
		for (VkFormat format : candidates)
		{
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
				return format;
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
				return format;
			}

			throw std::runtime_error("failed to find supported format!");
		}

		return candidates[0];
	}

	void VulkanSwapChain::Create()
	{
		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_VulkanState.m_Surface;
		createInfo.minImageCount = MaxFrameInFlight;
		createInfo.imageFormat = m_VulkanDevice->GetSwapChainSupport().format.format;
		createInfo.imageColorSpace = m_VulkanDevice->GetSwapChainSupport().format.colorSpace;
		createInfo.imageExtent = m_VulkanDevice->GetSwapChainSupport().surfaceSize;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

		uint32_t queueFamilyIndices[] = { m_VulkanDevice->GetQueueHelper().graphicqueuefamily.value(),
			m_VulkanDevice->GetQueueHelper().presentqueuefamily.value() };

		if (m_VulkanDevice->GetQueueHelper().graphicqueuefamily != m_VulkanDevice->GetQueueHelper().presentqueuefamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}

		createInfo.preTransform = m_VulkanDevice->GetSwapChainSupport().capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = m_VulkanDevice->GetSwapChainSupport().presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		VK_CHECK(vkCreateSwapchainKHR(m_VulkanState.m_Device, &createInfo, nullptr, &m_VulkanState.m_SwapChain));

		uint32_t imageCount = MaxFrameInFlight;
		vkGetSwapchainImagesKHR(m_VulkanState.m_Device, m_VulkanState.m_SwapChain, &imageCount, m_VulkanState.m_SwapChainImages.data());

		for (size_t i = 0; i < MaxFrameInFlight; i++) {

			// imageview
			{
				VkImageViewCreateInfo createInfo{};
				createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				createInfo.image = m_VulkanState.m_SwapChainImages[i];

				createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				createInfo.format = m_VulkanDevice->GetSwapChainSupport().format.format;

				createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

				createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				createInfo.subresourceRange.baseMipLevel = 0;
				createInfo.subresourceRange.levelCount = 1;
				createInfo.subresourceRange.baseArrayLayer = 0;
				createInfo.subresourceRange.layerCount = 1;

				VK_CHECK(vkCreateImageView(m_VulkanState.m_Device, &createInfo, nullptr, &m_VulkanState.m_SwapChainImageViews[i]));
			}

			// Sampler
			{
				VkSamplerCreateInfo samplerInfo{};
				samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
				samplerInfo.magFilter = VK_FILTER_LINEAR;
				samplerInfo.minFilter = VK_FILTER_LINEAR;
				samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

				samplerInfo.anisotropyEnable = VK_TRUE;

				VkPhysicalDeviceProperties properties{};
				vkGetPhysicalDeviceProperties(m_VulkanState.m_PhysicalDevice, &properties);

				samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
				samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
				samplerInfo.unnormalizedCoordinates = VK_FALSE;
				samplerInfo.compareEnable = VK_FALSE;
				samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

				samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
				samplerInfo.mipLodBias = 0.0f;
				samplerInfo.minLod = 0;
				samplerInfo.maxLod = static_cast<float>(0);

				VK_CHECK(vkCreateSampler(m_VulkanState.m_Device, &samplerInfo, nullptr, &m_VulkanState.m_SwapChainImageSamplers[i]));
			}
		}
	}

	void VulkanSwapChain::Destroy()
	{
		for (auto imageView : m_VulkanState.m_SwapChainImageViews) {
			vkDestroyImageView(m_VulkanState.m_Device, imageView, nullptr);
		}

		for (auto sampler : m_VulkanState.m_SwapChainImageSamplers) {
			vkDestroySampler(m_VulkanState.m_Device, sampler, nullptr);
		}

		vkDestroySwapchainKHR(m_VulkanState.m_Device, m_VulkanState.m_SwapChain, nullptr);
	}

	void VulkanSwapChain::CreateSyncObjects()
	{
		m_VulkanState.m_ImageSemaphore.resize(MaxFrameInFlight);
		m_VulkanState.m_QueueSemaphore.resize(MaxFrameInFlight);
		m_VulkanState.m_Fence.resize(MaxFrameInFlight);

		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MaxFrameInFlight; i++) {
			VK_CHECK(vkCreateSemaphore(m_VulkanState.m_Device, &semaphoreInfo, nullptr, &m_VulkanState.m_ImageSemaphore[i]));
			VK_CHECK(vkCreateSemaphore(m_VulkanState.m_Device, &semaphoreInfo, nullptr, &m_VulkanState.m_QueueSemaphore[i]));
			VK_CHECK(vkCreateFence(m_VulkanState.m_Device, &fenceInfo, nullptr, &m_VulkanState.m_Fence[i]));
		}
	}

	void VulkanSwapChain::DestroySyncObjects()
	{
		for (size_t i = 0; i < MaxFrameInFlight; i++) {
			vkDestroySemaphore(m_VulkanState.m_Device, m_VulkanState.m_ImageSemaphore[i], nullptr);
			vkDestroySemaphore(m_VulkanState.m_Device, m_VulkanState.m_QueueSemaphore[i], nullptr);
			vkDestroyFence(m_VulkanState.m_Device, m_VulkanState.m_Fence[i], nullptr);
		}
	}
}