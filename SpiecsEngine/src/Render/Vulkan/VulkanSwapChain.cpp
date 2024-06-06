/**
* @file VulkanSwapChain.cpp.
* @brief The VulkanSwapChain Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "VulkanSwapChain.h"

namespace Spiecs {

	VulkanSwapChain::VulkanSwapChain(
		VulkanState&                  vulkanState  , 
		std::shared_ptr<VulkanDevice> vulkanDevice
	)
		: VulkanObject(vulkanState)
		, m_VulkanDevice(vulkanDevice)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Create s SwapChian.
		*/
		Create();

		/**
		* @brief Create CreateSyncObjects.
		*/
		CreateSyncObjects();
	}

	VulkanSwapChain::~VulkanSwapChain()
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Destroy SyncObjects.
		*/
		DestroySyncObjects();

		/**
		* @brief Destroy the SwapChian.
		*/
		Destroy();
	}

	VkFormat VulkanSwapChain::FindDepthFormat(const VkPhysicalDevice& physicalDevice)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Call findSupportedFormat with specific parameters.
		*/
		return findSupportedFormat(
			physicalDevice, 
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
		);
	}

	VkFormat VulkanSwapChain::findSupportedFormat(
		const VkPhysicalDevice&      physicalDevice , 
		const std::vector<VkFormat>& candidates     , 
		VkImageTiling tiling                        , 
		VkFormatFeatureFlags features
	)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Iter all candidates.
		*/
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

			SPIECS_CORE_WARN("VulkanSwapChain::Not find a suitable format.");
		}

		/**
		* @brief Return one if not find.
		*/
		return candidates[0];
	}

	void VulkanSwapChain::Create()
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Instance a VkSwapchainCreateInfoKHR.
		*/
		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType                       = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface                     = m_VulkanState.m_Surface;                                             // With Specific Surface, we might not change surface size after all.
		createInfo.minImageCount               = MaxFrameInFlight;                                                    // 2.
		createInfo.imageFormat                 = m_VulkanDevice->GetSwapChainSupport().format.format;                 // RGBA8 Linear.
		createInfo.imageColorSpace             = m_VulkanDevice->GetSwapChainSupport().format.colorSpace;
		createInfo.imageExtent                 = m_VulkanDevice->GetSwapChainSupport().surfaceSize;
		createInfo.imageArrayLayers            = 1;                                                                   // Might be used with MultipViewPort.
		createInfo.imageUsage                  = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

		uint32_t queueFamilyIndices[2] = 
		{ 
			m_VulkanDevice->GetQueueHelper().graphicqueuefamily.value(),
			m_VulkanDevice->GetQueueHelper().presentqueuefamily.value() 
		};

		if (queueFamilyIndices[0] != queueFamilyIndices[1])
		{
			createInfo.imageSharingMode        = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount   = 2;
			createInfo.pQueueFamilyIndices     = queueFamilyIndices;
		}
		else 
		{
			createInfo.imageSharingMode        = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount   = 0;
			createInfo.pQueueFamilyIndices     = nullptr;
		}

		createInfo.preTransform                = m_VulkanDevice->GetSwapChainSupport().capabilities.currentTransform;
		createInfo.compositeAlpha              = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode                 = m_VulkanDevice->GetSwapChainSupport().presentMode;
		createInfo.clipped                     = VK_TRUE;
		createInfo.oldSwapchain                = VK_NULL_HANDLE;

		/**
		* @breif Create SwapChain.
		*/
		VK_CHECK(vkCreateSwapchainKHR(m_VulkanState.m_Device, &createInfo, nullptr, &m_VulkanState.m_SwapChain));
		VulkanDebugUtils::SetObjectName(VK_OBJECT_TYPE_SWAPCHAIN_KHR, m_VulkanState.m_SwapChain, m_VulkanState.m_Device, "SpiecsEngineSwapChainKHR");

		/**
		* @brief Get Swapchain images created by SwapChain automatically.
		*/
		uint32_t imageCount = MaxFrameInFlight;
		vkGetSwapchainImagesKHR(m_VulkanState.m_Device, m_VulkanState.m_SwapChain, &imageCount, m_VulkanState.m_SwapChainImages.data());

		/**
		* @breif Create SwapChianImage Resources.
		*/
		for (size_t i = 0; i < MaxFrameInFlight; i++) 
		{
			/**
			* @brief Create ImageView.
			*/
			{
				/**
				* @brief Instance a VkImageViewCreateInfo.
				*/
				VkImageViewCreateInfo createInfo{};
				createInfo.sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				createInfo.image    = m_VulkanState.m_SwapChainImages[i];

				createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				createInfo.format   = m_VulkanDevice->GetSwapChainSupport().format.format;

				createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

				createInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
				createInfo.subresourceRange.baseMipLevel   = 0;
				createInfo.subresourceRange.levelCount     = 1;
				createInfo.subresourceRange.baseArrayLayer = 0;
				createInfo.subresourceRange.layerCount     = 1;

				VulkanDebugUtils::SetObjectName(VK_OBJECT_TYPE_IMAGE, m_VulkanState.m_SwapChainImages[i], m_VulkanState.m_Device, "SwapChainImage");
				VK_CHECK(vkCreateImageView(m_VulkanState.m_Device, &createInfo, nullptr, &m_VulkanState.m_SwapChainImageViews[i]));
			}

			/**
			* @brief Create Sampler.
			*/
			{
				/**
				* @brief Instance a VkSamplerCreateInfo.
				*/
				VkSamplerCreateInfo samplerInfo{};
				samplerInfo.sType            = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
				samplerInfo.magFilter        = VK_FILTER_LINEAR;
				samplerInfo.minFilter        = VK_FILTER_LINEAR;
				samplerInfo.addressModeU     = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				samplerInfo.addressModeV     = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				samplerInfo.addressModeW     = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				samplerInfo.anisotropyEnable = VK_TRUE;

				VkPhysicalDeviceProperties properties{};
				vkGetPhysicalDeviceProperties(m_VulkanState.m_PhysicalDevice, &properties);

				samplerInfo.maxAnisotropy           = properties.limits.maxSamplerAnisotropy;
				samplerInfo.borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
				samplerInfo.unnormalizedCoordinates = VK_FALSE;
				samplerInfo.compareEnable           = VK_FALSE;
				samplerInfo.compareOp               = VK_COMPARE_OP_ALWAYS;

				samplerInfo.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR;
				samplerInfo.mipLodBias              = 0.0f;
				samplerInfo.minLod                  = 0.0f;
				samplerInfo.maxLod                  = static_cast<float>(0);

				VK_CHECK(vkCreateSampler(m_VulkanState.m_Device, &samplerInfo, nullptr, &m_VulkanState.m_SwapChainImageSamplers[i]));
			}
		}
	}

	void VulkanSwapChain::Destroy()
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Destroy ImageView.
		*/
		for (auto imageView : m_VulkanState.m_SwapChainImageViews) 
		{
			vkDestroyImageView(m_VulkanState.m_Device, imageView, nullptr);
		}

		/**
		* @brief Destroy Sampler.
		*/
		for (auto sampler : m_VulkanState.m_SwapChainImageSamplers) 
		{
			vkDestroySampler(m_VulkanState.m_Device, sampler, nullptr);
		}

		/**
		* @brief Destroy SwapChain.
		*/
		vkDestroySwapchainKHR(m_VulkanState.m_Device, m_VulkanState.m_SwapChain, nullptr);
	}

	void VulkanSwapChain::CreateSyncObjects()
	{
		SPIECS_PROFILE_ZONE;

		m_VulkanState.m_ImageSemaphore.resize(MaxFrameInFlight);
		m_VulkanState.m_QueueSemaphore.resize(MaxFrameInFlight);
		m_VulkanState.m_Fence.resize(MaxFrameInFlight);

		/**
		* @brief Instance a VkSemaphoreCreateInfo.
		*/
		VkSemaphoreCreateInfo semaphoreInfo {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		/**
		* @brief Instance a VkFenceCreateInfo.
		*/
		VkFenceCreateInfo fenceInfo {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		/**
		* @brief Create SyncObkects.
		*/
		for (size_t i = 0; i < MaxFrameInFlight; i++) 
		{
			VK_CHECK(vkCreateSemaphore(m_VulkanState.m_Device, &semaphoreInfo, nullptr, &m_VulkanState.m_ImageSemaphore[i]));
			VK_CHECK(vkCreateSemaphore(m_VulkanState.m_Device, &semaphoreInfo, nullptr, &m_VulkanState.m_QueueSemaphore[i]));
			VK_CHECK(vkCreateFence(m_VulkanState.m_Device, &fenceInfo, nullptr, &m_VulkanState.m_Fence[i]));
		}
	}

	void VulkanSwapChain::DestroySyncObjects()
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Destroy SyncObkects.
		*/
		for (size_t i = 0; i < MaxFrameInFlight; i++) 
		{
			vkDestroySemaphore(m_VulkanState.m_Device, m_VulkanState.m_ImageSemaphore[i], nullptr);
			vkDestroySemaphore(m_VulkanState.m_Device, m_VulkanState.m_QueueSemaphore[i], nullptr);
			vkDestroyFence(m_VulkanState.m_Device, m_VulkanState.m_Fence[i], nullptr);
		}
	}
}