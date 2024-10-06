/**
* @file VulkanDevice.h.
* @brief The VulkanDevice Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

#include <optional>

namespace Spices {

	/**
	* @brief This struct contains all queues's identify we need. 
	*/
	struct QueueHelper
	{
		/**
		* @brief The graphic queue's in used identify.
		*/
		std::optional<uint32_t> graphicqueuefamily;

		/**
		* @brief The present queue's in used identify.
		*/
		std::optional<uint32_t> presentqueuefamily;

		/**
		* @brief The transfer queue's in used identify.
		*/
		std::optional<uint32_t> transferqueuefamily;

		/**
		* @brief The compute queue's in used identify.
		*/
		std::optional<uint32_t> computequeuefamily;

		/**
		* @brief Whether all queues that we need is valid.
		*/
		bool isComplete() const
		{
			return 

			/**
			* @brief Whether graphicqueue is supported by device.
			*/
			graphicqueuefamily.has_value()  &&

			/**
			* @brief Whether presentqueue is supported by device.
			*/
			presentqueuefamily.has_value()  &&

			/**
			* @brief Whether transferqueu is supported by device.
			*/
			transferqueuefamily.has_value() &&

			/**
			* @brief Whether computequeue is supported by device.
			*/
			computequeuefamily.has_value()  ;
		}
	};

	/**
	* @brief SwapChain Utils.
	* Queried from device.
	*/
	struct SwapChainSupportDetails 
	{
		/**
		* @brief VkSurfaceCapabilitiesKHR.
		*/
		VkSurfaceCapabilitiesKHR capabilities;

		/**
		* @brief All supported VkSurfaceFormatKHR.
		*/
		std::vector<VkSurfaceFormatKHR> formats;

		/**
		* @brief All supported VkPresentModeKHR.
		*/
		std::vector<VkPresentModeKHR> presentModes;

		/**
		* @brief The selected VkSurfaceFormatKHR.
		*/
		VkSurfaceFormatKHR format;

		/**
		* @brief The selected VkPresentModeKHR.
		*/
		VkPresentModeKHR presentMode;

		/**
		* @brief The VkSurface Size.
		*/
		VkExtent2D surfaceSize;

		/**
		* @brief The ViewPort Size.
		* Init value. Reset by resize event.
		*/
		VkExtent2D viewPortSize = { 1280, 960 };
	};

	/**
	* @brief VulkanInstance Class.
	* This class defines the VulkanDevice behave.
	* This class is just a wrapper of vkdevice.
	*/
	class VulkanDevice : public VulkanObject
	{
	public:

		/**
		* @brief Constructor Function.
		* Create vkdevice.
		* @param[in] vulkanState The global VulkanState.
		*/
		VulkanDevice(VulkanState& vulkanState);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanDevice() override;

		/**
		* @brief Get SwapChain Utils.
		* @return Returns SwapChain Utils.
		*/
		inline const SwapChainSupportDetails& GetSwapChainSupport() { return m_SwapChainSupportDetails; }

		/**
		* @brief Requery device's SwapChainSupportDetails.
		* Mainly VkExtent2D.
		*/
		void RequerySwapChainSupport();

		/**
		* @brief Get QueueHelper variable.
		* @return Returns QueueHelper variable.
		*/
		inline const QueueHelper& GetQueueHelper() const { return m_QueueHelper; }

		/**
		* @brief Get device's maxusablesamplecount.
		* @return Returns device's maxusablesamplecount.
		*/
		VkSampleCountFlagBits GetMaxUsableSampleCount() const;

		/**
		* @brief Get RayTracingPipelineProperties.
		* @return Returns RayTracingPipelineProperties.
		*/
		inline static VkPhysicalDeviceRayTracingPipelinePropertiesKHR& GetRTPipelineProperties() { return m_RayTracingProperties; }

		/**
		* @brief Get DeviceGeneratedCommandsPropertiesNV.
		* @return Returns DeviceGeneratedCommandsPropertiesNV.
		*/
		inline static VkPhysicalDeviceDeviceGeneratedCommandsPropertiesNV& GetDGCProperties() { return m_DGCProperties; };

		/**
		* @brief Get VkPhysicalDeviceFeatures.
		* @return Returns VkPhysicalDeviceFeatures.
		*/
		inline static VkPhysicalDeviceFeatures& GetDeviceFeatures() { return m_DeviceFeatures; };

	private:

		/**
		* @brief select a suitable physical device.
		* @param[in] instance VkInstance.
		* @param[in] surface VkSurfaceKHR.
		* @param[in] window The row pointer of GLFW window.
		* @return Returns true if select one successfully.
		* @todo multiple physical device support.
		*/
		bool SelectPhysicalDevice(
			const VkInstance&   instance , 
			const VkSurfaceKHR& surface  , 
			GLFWwindow*         window
		);

		/**
		* @brief Check all Property we need meet.
		* @param[in] device VkPhysicalDevice.
		* @return Returns true if all Property we need meet.
		*/
		bool IsPropertyMeetDemand(const VkPhysicalDevice& device);

		/**
		* @brief Check all  Feature we need meet.
		* @param[in] device VkPhysicalDevice.
		* @return Returns true if all Feature we need meet.
		*/
		bool IsFeatureMeetDemand(const VkPhysicalDevice& device);

		/**
		* @brief Get all physical device extension requirements our engine needed.
		* Source 1 : user Setting.
		* @todo Configurable
		*/
		void GetExtensionRequirements();

		/**
		* @brief Check all Extension we need meet;
		* @param[in] device VkPhysicalDevice.
		* @return Returns true if all Extension we need meet.
		*/
		bool IsExtensionMeetDemand(const VkPhysicalDevice& device);

		/**
		* @brief Check all Queue we need meet;
		* @param[in] device VkPhysicalDevice.
		* @param[in] surface VkSurfaceKHR.
		* @return Returns true if all Extension we need meet.
		*/
		bool IsQueueMeetDemand(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);

		/**
		* @brief Query physicaldevice's SwapChainSupport.
		* @param[in] device VkPhysicalDevice.
		* @param[in] surface VkSurfaceKHR.
		* @param[in] window The row pointer of GLFWwindow.
		* @return Returns SwapChainSupportDetails.
		*/
		static SwapChainSupportDetails QuerySwapChainSupport(
			const VkPhysicalDevice& device  , 
			const VkSurfaceKHR&     surface , 
			GLFWwindow*             window
		);

	private:

		/**
		* @brief Device Extension Properties.
		*/
		std::vector<const char*> m_ExtensionProperties;

		/**
		* @brief Selected Physical Device Properties.
		*/
		static VkPhysicalDeviceProperties m_DeviceProperties;

		/**
		* @brief Selected Physical Device Features.
		*/
		static VkPhysicalDeviceFeatures m_DeviceFeatures;

		/**
		* @brief Device RayTracing Properties.
		*/
		static VkPhysicalDeviceRayTracingPipelinePropertiesKHR m_RayTracingProperties;

		/**
		* @brief Device DGC Properties.
		*/
		static VkPhysicalDeviceDeviceGeneratedCommandsPropertiesNV m_DGCProperties;

		/**
		* @brief QueueHelper.
		*/
		QueueHelper m_QueueHelper;

		/**
		* @brief SwapChainSupportDetails.
		*/
		SwapChainSupportDetails m_SwapChainSupportDetails{};
	};
}
