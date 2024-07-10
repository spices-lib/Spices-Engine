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
	class VulkanSwapChain : public VulkanObject
	{
	public:

		/**
		* @brief Constructor Function.
		* Create vkinstance and vksurface.
		* @param[in] vulkanState The global VulkanState.
		* @param[in] vulkanDevice The shared pointer of VulkanDevice.
		*/
		VulkanSwapChain(
			VulkanState&                  vulkanState  , 
			std::shared_ptr<VulkanDevice> vulkanDevice
		);

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
		static VkFormat findSupportedFormat(
			const VkPhysicalDevice&      physicalDevice , 
			const std::vector<VkFormat>& candidates     , 
			VkImageTiling                tiling         , 
			VkFormatFeatureFlags         features
		);

		/**
		* @brief Create this.
		*/
		void Create() const;

		/**
		* @brief Destroy this.
		*/
		void Destroy() const;

	private:

		/**
		* @brief Create Sync Objects
		* @todo Move to another file.
		*/
		void CreateSyncObjects() const;

		/**
		* @brief Destroy Sync Objects
		*/
		void DestroySyncObjects() const;

	private:

		/**
		* @brief The shared pointer of VulkanDevice.
		*/
		std::shared_ptr<VulkanDevice> m_VulkanDevice;
	};
}