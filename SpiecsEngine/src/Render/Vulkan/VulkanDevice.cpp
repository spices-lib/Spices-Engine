#include "pchheader.h"
#include "VulkanDevice.h"

namespace Spiecs {

	VulkanDevice::VulkanDevice(VulkanState& vulkanState)
		: VulkanObject(vulkanState)
	{
		if (!SelectPhysicalDevice(vulkanState.m_Instance, vulkanState.m_Surface, vulkanState.m_Windows)) {
			SPIECS_LOG("failed select physical device!");
		}

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { m_QueueHelper.graphicqueuefamily.value(), m_QueueHelper.presentqueuefamily.value(), m_QueueHelper.computequeuefamily.value(), m_QueueHelper.transferqueuefamily.value() };

		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;

			float queuePriority = 1.0f;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};
		deviceFeatures.samplerAnisotropy = VK_TRUE; // 启用纹理采样的各项异性
		deviceFeatures.sampleRateShading = VK_TRUE; // 启用实例着色（应对MSAA的缺点）

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pEnabledFeatures = &deviceFeatures;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(m_ExtensionProperties.size());
		createInfo.ppEnabledExtensionNames = m_ExtensionProperties.data();
		createInfo.enabledLayerCount = 0;

		VK_CHECK(vkCreateDevice(vulkanState.m_PhysicalDevice, &createInfo, nullptr, &vulkanState.m_Device));
		SPIECS_LOG("VkDevice created succeed!!!");

		vkGetDeviceQueue(vulkanState.m_Device, m_QueueHelper.graphicqueuefamily.value(), 0, &vulkanState.m_GraphicQueue);
		vkGetDeviceQueue(vulkanState.m_Device, m_QueueHelper.presentqueuefamily.value(), 0, &vulkanState.m_PresentQueue);
		vkGetDeviceQueue(vulkanState.m_Device, m_QueueHelper.computequeuefamily.value(), 0, &vulkanState.m_ComputeQueue);
		vkGetDeviceQueue(vulkanState.m_Device, m_QueueHelper.transferqueuefamily.value(), 0, &vulkanState.m_TransformQueue);
	}

	VulkanDevice::~VulkanDevice()
	{
		vkDestroyDevice(m_VulkanState.m_Device, nullptr);
	}

	void VulkanDevice::RequerySwapChainSupport()
	{
		m_SwapChainSupportDetails = QuerySwapChainSupport(m_VulkanState.m_PhysicalDevice, m_VulkanState.m_Surface, m_VulkanState.m_Windows);
	}

	VkSampleCountFlagBits VulkanDevice::GetMaxUsableSampleCount()
	{
		VkPhysicalDeviceProperties physicalDeviceProperties;
		vkGetPhysicalDeviceProperties(m_VulkanState.m_PhysicalDevice, &physicalDeviceProperties);

		VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;

		if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
		if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
		if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
		if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
		if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
		if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

		return VK_SAMPLE_COUNT_1_BIT;
	}

	bool VulkanDevice::SelectPhysicalDevice(const VkInstance& instance, const VkSurfaceKHR& surface, GLFWwindow* window)
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

		if (deviceCount == 0) {
			throw std::runtime_error("failed to find GPUs with Vulkan support!");
			return false;
		}

		std::vector<VkPhysicalDevice> PhysicalDevices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, PhysicalDevices.data());

		bool find = false;
		for (const auto& physicalDevice : PhysicalDevices) {
			find = IsPropertyMeetDemand(physicalDevice) &&
				IsFeatureMeetDemand(physicalDevice) &&
				IsExtensionMeetDemand(physicalDevice) &&
				IsQueueMeetDemand(physicalDevice, surface);

			if (find)
			{
				m_VulkanState.m_PhysicalDevice = physicalDevice;
				m_SwapChainSupportDetails = QuerySwapChainSupport(physicalDevice, surface, window);
				SPIECS_LOG("VkPhysicalDevice Selected!!!");
				break;
			}
		}

		return find;
	}

	bool VulkanDevice::IsPropertyMeetDemand(const VkPhysicalDevice& device)
	{
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);

		return true;
	}

	bool VulkanDevice::IsFeatureMeetDemand(const VkPhysicalDevice& device)
	{
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		return true;
	}

	bool VulkanDevice::IsExtensionMeetDemand(const VkPhysicalDevice& device)
	{
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(m_ExtensionProperties.begin(), m_ExtensionProperties.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	bool VulkanDevice::IsQueueMeetDemand(const VkPhysicalDevice& device, const VkSurfaceKHR& surface)
	{
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				m_QueueHelper.graphicqueuefamily = i;

				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
				if (presentSupport) {
					m_QueueHelper.presentqueuefamily = i;
				}

				if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
					m_QueueHelper.computequeuefamily = i;
				}

				if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) {
					m_QueueHelper.transferqueuefamily = i;
				}
			}

			if (m_QueueHelper.isComplete()) break;
			i++;
		}

		return m_QueueHelper.isComplete();
	}

	SwapChainSupportDetails VulkanDevice::QuerySwapChainSupport(const VkPhysicalDevice& device, const VkSurfaceKHR& surface, GLFWwindow* window)
	{
		SwapChainSupportDetails swapChainSupportDetails = SwapChainSupportDetails{};
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &swapChainSupportDetails.capabilities);

		if (swapChainSupportDetails.capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			swapChainSupportDetails.extent = swapChainSupportDetails.capabilities.currentExtent;
		}
		else
		{
			int width, height;
			glfwGetFramebufferSize(window, &width, &height);

			VkExtent2D actualExtent = {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
			};

			actualExtent.width = std::clamp(actualExtent.width, swapChainSupportDetails.capabilities.minImageExtent.width,
				swapChainSupportDetails.capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, swapChainSupportDetails.capabilities.minImageExtent.height,
				swapChainSupportDetails.capabilities.maxImageExtent.height);

			swapChainSupportDetails.extent = actualExtent;
		}

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

		if (formatCount != 0) {
			swapChainSupportDetails.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, swapChainSupportDetails.formats.data());

			bool find = false;
			for (const auto& availableFormat : swapChainSupportDetails.formats) {
				if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
					availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {

					swapChainSupportDetails.format = availableFormat;
					find = true;
				}
			}

			if (!find)
			{
				swapChainSupportDetails.format = swapChainSupportDetails.formats[0];
			}
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

		if (presentModeCount != 0)
		{
			swapChainSupportDetails.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, swapChainSupportDetails.presentModes.data());

			bool find = false;
			for (const auto& availablePresentMode : swapChainSupportDetails.presentModes) {
				if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
					swapChainSupportDetails.presentMode = availablePresentMode;
					find = true;
					break;
				}
			}

			if (!find)
			{
				swapChainSupportDetails.presentMode = swapChainSupportDetails.presentModes[0];
			}
		}

		return swapChainSupportDetails;
	}

}