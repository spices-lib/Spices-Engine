#include "pchheader.h"
#include "VulkanSwapChain.h"

namespace Spiecs {

	VulkanSwapChain::VulkanSwapChain(VulkanState& vulkanState)
		: VulkanObject(vulkanState)
	{
		Create();
	}

	VulkanSwapChain::~VulkanSwapChain()
	{
		Destroy();
	}

	void VulkanSwapChain::Create()
	{
		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_VulkanState.m_Surface;
		createInfo.minImageCount = MaxFrameInFlight;
		createInfo.imageFormat = m_Device->GetSwapChainSupport().format.format;
		createInfo.imageColorSpace = m_Device->GetSwapChainSupport().format.colorSpace;
		createInfo.imageExtent = m_Device->GetSwapChainSupport().extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		uint32_t queueFamilyIndices[] = { m_Device->GetQueueHelper().graphicqueuefamily.value(),
			m_Device->GetQueueHelper().presentqueuefamily.value() };

		if (m_Device->GetQueueHelper().graphicqueuefamily != m_Device->GetQueueHelper().presentqueuefamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}

		createInfo.preTransform = m_Device->GetSwapChainSupport().capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = m_Device->GetSwapChainSupport().presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		VK_CHECK(vkCreateSwapchainKHR(*m_Device, &createInfo, nullptr, &m_SwapChain));
		FL_LOG("VkSwapchainKHR create succeed!!!");

		uint32_t imageCount = MaxFramInFlight;
		vkGetSwapchainImagesKHR(*m_Device, m_SwapChain, &imageCount, m_SwapChainImages.data());
		FL_LOG("SwapchainImages getted!!!");

		for (size_t i = 0; i < m_SwapChainImages.size(); i++) {

			// imageview
			{
				VkImageViewCreateInfo createInfo{};
				createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				createInfo.image = m_SwapChainImages[i];

				createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				createInfo.format = m_Device->GetSwapChainSupport().format.format;

				createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

				createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				createInfo.subresourceRange.baseMipLevel = 0;
				createInfo.subresourceRange.levelCount = 1;
				createInfo.subresourceRange.baseArrayLayer = 0;
				createInfo.subresourceRange.layerCount = 1;

				VK_CHECK(vkCreateImageView(*m_Device, &createInfo, nullptr, &m_SwapChainImageViews[i]));
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
				vkGetPhysicalDeviceProperties(m_Device->GetPhysicalDevice(), &properties);

				samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
				samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
				samplerInfo.unnormalizedCoordinates = VK_FALSE;
				samplerInfo.compareEnable = VK_FALSE;
				samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

				samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
				samplerInfo.mipLodBias = 0.0f;
				samplerInfo.minLod = 0;
				samplerInfo.maxLod = static_cast<float>(0);

				VK_CHECK(vkCreateSampler(*m_Device, &samplerInfo, nullptr, &m_SwapChainImageSamplers[i]));
			}
		}
		FL_LOG("SwapchainImageViews created succeed!!!");

		{
			VkFormat colorFormat = m_Device->GetSwapChainSupport().format.format;

			m_ColorImage = new Image(
				m_Device,
				m_Device->GetSwapChainSupport().extent.width,
				m_Device->GetSwapChainSupport().extent.height,
				m_Device->GetMaxUsableSampleCount(),
				colorFormat,
				VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
				1);

			m_ColorImage->CreateImageView(colorFormat, VK_IMAGE_ASPECT_COLOR_BIT);
			m_ColorImage->CreateSampler();
		}

		{
			VkFormat depthFormat = FindDepthFormat(m_Device->GetPhysicalDevice());

			m_DepthImage = new Image(
				m_Device,
				m_Device->GetSwapChainSupport().extent.width,
				m_Device->GetSwapChainSupport().extent.height,
				m_Device->GetMaxUsableSampleCount(),
				depthFormat,
				VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
				1
			);
			m_DepthImage->CreateImageView(depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
			m_DepthImage->TransitionImageLayout(
				depthFormat,
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
			);
			m_DepthImage->CreateSampler();
		}

		for (size_t i = 0; i < m_SwapChainImageViews.size(); i++) {
			std::array<VkImageView, 3> attachments[] = { m_ColorImage->GetView(), m_DepthImage->GetView(), m_SwapChainImageViews[i] };

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = *m_RenderPass;
			framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments->size());
			framebufferInfo.pAttachments = attachments->data();
			framebufferInfo.width = m_Device->GetSwapChainSupport().extent.width;
			framebufferInfo.height = m_Device->GetSwapChainSupport().extent.height;
			framebufferInfo.layers = 1;

			VK_CHECK(vkCreateFramebuffer(*m_Device, &framebufferInfo, nullptr, &m_SwapChainFramebuffers[i]));
		}
	}

	void VulkanSwapChain::Destroy()
	{
		for (auto framebuffer : m_SwapChainFramebuffers) {
			vkDestroyFramebuffer(m_VulkanState.m_Device, framebuffer, nullptr);
		}

		for (auto imageView : m_SwapChainImageViews) {
			vkDestroyImageView(m_VulkanState.m_Device, imageView, nullptr);
		}

		for (auto sampler : m_SwapChainImageSamplers) {
			vkDestroySampler(m_VulkanState.m_Device, sampler, nullptr);
		}

		vkDestroySwapchainKHR(m_VulkanState.m_Device, m_VulkanState.m_SwapChain, nullptr);
	}
}