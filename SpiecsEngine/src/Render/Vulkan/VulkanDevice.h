#pragma once
#include "Core/Core.h"
#include "optional"

namespace Spiecs {

	struct QueueHelper
	{
		std::optional<uint32_t> graphicqueuefamily;
		std::optional<uint32_t> presentqueuefamily;
		std::optional<uint32_t> transferqueuefamily;
		std::optional<uint32_t> computequeuefamily;

		VkQueue graphicqueue;
		VkQueue presentqueue;
		VkQueue transferqueue;
		VkQueue computequeue;

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

	class VulkanDevice
	{
	public:
		VulkanDevice(const VkInstance& instance, VkSurfaceKHR* surface, GLFWwindow* window);
		virtual ~VulkanDevice();

	private:
		// only select one suitable physicaldevice
		VkPhysicalDevice m_PhysicalDevice;

		// logical device
		VkDevice m_Device;

		// queue 
		QueueHelper m_QueueHelper;

		// swapchain helper
		SwapChainSupportDetails m_SwapChainSupportDetails;
	};
}
