#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

#include "optional"

namespace Spiecs {

	struct QueueHelper
	{
		std::optional<uint32_t> graphicqueuefamily;
		std::optional<uint32_t> presentqueuefamily;
		std::optional<uint32_t> transferqueuefamily;
		std::optional<uint32_t> computequeuefamily;

		bool isComplete() {
			return graphicqueuefamily.has_value() &&
				presentqueuefamily.has_value() &&
				transferqueuefamily.has_value() &&
				computequeuefamily.has_value();
		}
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;

		VkSurfaceFormatKHR format;
		VkPresentModeKHR presentMode;
		VkExtent2D extent;
	};

	class VulkanDevice : public VulkanObject
	{
	public:
		VulkanDevice(VulkanState& vulkanState);
		virtual ~VulkanDevice();

		SwapChainSupportDetails& GetSwapChainSupport() { return m_SwapChainSupportDetails; };
		void RequerySwapChainSupport();
		QueueHelper& GetQueueHelper() { return m_QueueHelper; };
		VkSampleCountFlagBits GetMaxUsableSampleCount();

	private:
		bool SelectPhysicalDevice(const VkInstance& instance, const VkSurfaceKHR& surface, GLFWwindow* window);
		bool IsPropertyMeetDemand(const VkPhysicalDevice& device);
		bool IsFeatureMeetDemand(const VkPhysicalDevice& device);
		bool IsExtensionMeetDemand(const VkPhysicalDevice& device);
		bool IsQueueMeetDemand(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);
		SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& device, const VkSurfaceKHR& surface, GLFWwindow* window);
	private:

		std::vector<const char*> m_ExtensionProperties = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,     // Swapchain Extension
			VK_KHR_MAINTENANCE_1_EXTENSION_NAME, // Negative Viewpoet Extension
		};

		// queue 
		QueueHelper m_QueueHelper;

		// swapchain helper
		SwapChainSupportDetails m_SwapChainSupportDetails{};
	};
}
