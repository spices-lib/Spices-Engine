/**
* @file VulkanSwapChain.h.
* @brief The VulkanSwapChain Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"
#include "VulkanImage.h"
#include "VulkanDevice.h"

namespace Spiecs {

	/**
	* @brief VulkanSwapChain Class.
	* This class defines the VulkanSwapChain behaver.
	* This class is just a wapper of VkSwapchainKHR.
	*/
	class VulkanSwapChain : VulkanObject
	{
	public:

		/**
		* @brief Constructor Function.
		* Create vkinstance and vksurface.
		* @param[in] vulkanState The global VulkanState.
		* @param[in] vulkanDevice The shared pointer of VulkanDevice.
		*/
		VulkanSwapChain(VulkanState& vulkanState, std::shared_ptr<VulkanDevice> vulkanDevice);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanSwapChain();

		/**
		* @brief Check whether Depth Image's Format is supported by physicaldevice.
		* @param[in] physicalDevice VkPhysicalDevice.
		* @return Returns the Depth Image's Format
		*/
		static VkFormat FindDepthFormat(const VkPhysicalDevice& physicalDevice);

		/**
		* @brief Check whether specific formats is supported by physicaldevice.
		* @param[in] physicalDevice VkPhysicalDevice.
		* @param[in] candidates The VkFormats Container we need query.
		* @param[in] tiling VkImageTiling.
		* @param[in] features VkFormatFeatureFlags.
		* @return Returns a Format supported.
		*/
		static VkFormat findSupportedFormat(const VkPhysicalDevice& physicalDevice, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

		/**
		* @brief Get Framebuffer by index.
		* @param[in] index Frameindex in FrameInfo.
		* @return Returns the indexed Framebuffer.
		*/
		inline VkFramebuffer& GetFramebuffer(uint32_t index) { return m_SwapChainFramebuffers[index]; };

		/**
		* @brief Create this.
		*/
		void Create();

		/**
		* @brief Destroy this.
		*/
		void Destroy();

	private:

		/**
		* @brief Create Sync Objects
		* @todo Move to another file.
		*/
		void CreateSyncObjects();

		/**
		* @brief Destroy Sync Objects
		*/
		void DestroySyncObjects();

	private:

		/**
		* @brief The shared pointer of VulkanDevice.
		*/
		std::shared_ptr<VulkanDevice> m_VulkanDevice;

		/**
		* @brief The SwapChain's image, used for present.
		*/
		std::array<VkImage, MaxFrameInFlight> m_SwapChainImages;

		/**
		* @brief The SwapChain's imageView.
		*/
		std::array<VkImageView, MaxFrameInFlight> m_SwapChainImageViews;

		/**
		* @brief The SwapChain's imageSampler.
		*/
		std::array<VkSampler, MaxFrameInFlight> m_SwapChainImageSamplers;

		/**
		* @brief The FrameBuffer's attachment, used for MSAA.
		*/
		std::unique_ptr<VulkanImage> m_ColorImage;

		/**
		* @brief The FrameBuffer's attachment, used for Depth.
		*/
		std::unique_ptr<VulkanImage> m_DepthImage;

		// m_SwapChainImages[i]

		/**
		* @brief The FrameBuffers.
		*/
		std::array<VkFramebuffer, MaxFrameInFlight> m_SwapChainFramebuffers;
	};
}