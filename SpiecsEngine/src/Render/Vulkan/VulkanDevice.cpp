/**
* @file VulkanDevice.cpp.
* @brief The VulkanDevice Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "VulkanDevice.h"

namespace Spiecs {

	VulkanDevice::VulkanDevice(VulkanState& vulkanState)
		: VulkanObject(vulkanState)
	{
		/**
		* @brief Select one suitable physical device.
		*/
		if (!SelectPhysicalDevice(vulkanState.m_Instance, vulkanState.m_Surface, vulkanState.m_Windows)) {
			SPIECS_LOG("failed select physical device!");
		}

		/**
		* @brief Create a queue identifies container.
		*/
		std::set<uint32_t> uniqueQueueFamilies = { m_QueueHelper.graphicqueuefamily.value(), m_QueueHelper.presentqueuefamily.value(), m_QueueHelper.computequeuefamily.value(), m_QueueHelper.transferqueuefamily.value() };

		/**
		* @brief Fill in VkDeviceQueueCreateInfo.
		*/
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		for (uint32_t queueFamily : uniqueQueueFamilies) 
		{
			/**
			* @brief Instanced a VkDeviceQueueCreateInfo with default value.
			*/
			VkDeviceQueueCreateInfo queueCreateInfo{};

			/**
			* @brief Fill in sType.
			*/
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

			/**
			* @brief Fill in sType.
			*/
			queueCreateInfo.queueFamilyIndex = queueFamily;

			/**
			* @brief Fill in sType.
			*/
			queueCreateInfo.queueCount = 1;

			/**
			* @brief Fill in sType.
			*/
			float queuePriority = 1.0f;
			queueCreateInfo.pQueuePriorities = &queuePriority;

			queueCreateInfos.push_back(queueCreateInfo);
		}

		/**
		* @brief Instanced a VkDeviceCreateInfo with default value.
		*/
		VkDeviceCreateInfo createInfo{};

		/**
		* @brief Fill in sType.
		*/
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		/**
		* @brief Fill in pQueueCreateInfos.
		*/
		createInfo.pQueueCreateInfos = queueCreateInfos.data();

		/**
		* @brief Fill in queueCreateInfoCount.
		*/
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());

		/**
		* @brief Fill in pEnabledFeatures.
		*/
		createInfo.pEnabledFeatures = &m_DeviceFeatures;

		/**
		* @brief Fill in enabledExtensionCount.
		*/
		createInfo.enabledExtensionCount = static_cast<uint32_t>(m_ExtensionProperties.size());

		/**
		* @brief Fill in ppEnabledExtensionNames.
		*/
		createInfo.ppEnabledExtensionNames = m_ExtensionProperties.data();

		/**
		* @brief Fill in enabledLayerCount.
		*/
		createInfo.enabledLayerCount = 0;

		/**
		* @brief Create device and set it global.
		*/
		VK_CHECK(vkCreateDevice(vulkanState.m_PhysicalDevice, &createInfo, nullptr, &vulkanState.m_Device));

		/**
		* @brief Get graphic queue and set it global.
		*/
		vkGetDeviceQueue(vulkanState.m_Device, m_QueueHelper.graphicqueuefamily.value(), 0, &vulkanState.m_GraphicQueue);

		/**
		* @brief Get present queue and set it global.
		*/
		vkGetDeviceQueue(vulkanState.m_Device, m_QueueHelper.presentqueuefamily.value(), 0, &vulkanState.m_PresentQueue);

		/**
		* @brief Get compute queue and set it global.
		*/
		vkGetDeviceQueue(vulkanState.m_Device, m_QueueHelper.computequeuefamily.value(), 0, &vulkanState.m_ComputeQueue);

		/**
		* @brief Get transfer queue and set it global.
		*/
		vkGetDeviceQueue(vulkanState.m_Device, m_QueueHelper.transferqueuefamily.value(), 0, &vulkanState.m_TransformQueue);
	}

	VulkanDevice::~VulkanDevice()
	{
		/**
		* @brief Destroy the Vulkan Device Object.
		* Queue is created by device, we do not need destroy queue here manually.
		*/
		vkDestroyDevice(m_VulkanState.m_Device, nullptr);
	}

	void VulkanDevice::RequerySwapChainSupport()
	{
		m_SwapChainSupportDetails = QuerySwapChainSupport(m_VulkanState.m_PhysicalDevice, m_VulkanState.m_Surface, m_VulkanState.m_Windows);
	}

	VkSampleCountFlagBits VulkanDevice::GetMaxUsableSampleCount()
	{
		/**
		* @brief Get VkPhysicalDeviceProperties.
		*/
		VkPhysicalDeviceProperties physicalDeviceProperties;
		vkGetPhysicalDeviceProperties(m_VulkanState.m_PhysicalDevice, &physicalDeviceProperties);

		VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;

		/**
		* @breif select one.
		*/
		if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
		if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
		if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
		if (counts & VK_SAMPLE_COUNT_8_BIT)  { return VK_SAMPLE_COUNT_8_BIT;  }
		if (counts & VK_SAMPLE_COUNT_4_BIT)  { return VK_SAMPLE_COUNT_4_BIT;  }
		if (counts & VK_SAMPLE_COUNT_2_BIT)  { return VK_SAMPLE_COUNT_2_BIT;  }

		return VK_SAMPLE_COUNT_1_BIT;
	}

	bool VulkanDevice::SelectPhysicalDevice(const VkInstance& instance, const VkSurfaceKHR& surface, GLFWwindow* window)
	{
		/**
		* @brief Get all physical device num this computer.
		*/
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

		/**
		* @brief This software requires there must be at least one physical device.
		*/
		if (deviceCount == 0) {
			throw std::runtime_error("failed to find GPUs with Vulkan support!");
			return false;
		}

		/**
		* @breif Get all physical device this computer.
		*/
		std::vector<VkPhysicalDevice> PhysicalDevices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, PhysicalDevices.data());

		/**
		* @brief Iter all physical and select one suitable.
		*/
		for (const auto& physicalDevice : PhysicalDevices) 
		{
			/**
			* @brief All this condition need satisfied.
			*/
			if (IsPropertyMeetDemand(physicalDevice) && IsFeatureMeetDemand(physicalDevice) && IsExtensionMeetDemand(physicalDevice) && IsQueueMeetDemand(physicalDevice, surface))
			{
				/**
				* @brief Set Selected physical device.
				*/
				m_VulkanState.m_PhysicalDevice = physicalDevice;

				/**
				* @brief Query physicaldevice's SwapChainSupport.
				*/
				m_SwapChainSupportDetails = QuerySwapChainSupport(physicalDevice, surface, window);

				SPIECS_LOG("VkPhysicalDevice Selected!!!");
				return true;
			}
		}

		return false;
	}

	bool VulkanDevice::IsPropertyMeetDemand(const VkPhysicalDevice& device)
	{
		/**
		* @brief Get all Properties that supported.
		*/
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);

		/**
		* @brief Just return true for we do not need a specific property supported now.
		* @todo Configurable.
		*/
		return true;
	}

	void VulkanDevice::GetFeatureRequirements()
	{
		/**
		* @brief 启用纹理采样的各项异性
		*/
		m_DeviceFeatures.samplerAnisotropy = VK_TRUE;

		/**
		* @brief 启用实例着色（应对MSAA的缺点）
		*/
		m_DeviceFeatures.sampleRateShading = VK_TRUE;
	}

	bool VulkanDevice::IsFeatureMeetDemand(const VkPhysicalDevice& device)
	{
		/**
		* @brief Get all Features that supported.
		*/
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		/**
		* @brief Just return true for we do not need a specific feature supported now.
		* @todo Configurable.
		*/
		GetFeatureRequirements();

		return true;
	}

	void VulkanDevice::GetExtensionRequirements()
	{
		/**
		* @brief Swapchain Extension.
		*/
		m_ExtensionProperties.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		/**
		* @brief Negative Viewpoet Extension
		*/
		m_ExtensionProperties.push_back(VK_KHR_MAINTENANCE_1_EXTENSION_NAME);
	}

	bool VulkanDevice::IsExtensionMeetDemand(const VkPhysicalDevice& device)
	{
		/**
		* @brief Get all physicaldevice extensions nums.
		*/
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		/**
		* @brief Get all physicaldevice extensions that supported.
		*/
		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		/**
		* @brief Get all physicaldevice extension requirements our engine needed.
		*/
		GetExtensionRequirements();

		/**
		* @brief Check whether all extensions satisfied.
		*/
		std::set<std::string> requiredExtensions(m_ExtensionProperties.begin(), m_ExtensionProperties.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	bool VulkanDevice::IsQueueMeetDemand(const VkPhysicalDevice& device, const VkSurfaceKHR& surface)
	{
		/**
		* @brief Get all physicaldevice queue nums.
		*/
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		/**
		* @brief Get all physicaldevice queue that supported.
		*/
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		/**
		* @brief Check queue identify.
		*/
		for (int i = 0; i < queueFamilies.size(); i++) {
			auto& queueFamily = queueFamilies[i];

			/**
			* @brief Get graphic queue identify.
			*/
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				m_QueueHelper.graphicqueuefamily = i;
				m_VulkanState.m_GraphicQueueFamily = i;

				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

				/**
				* @brief Get present queue identify.
				*/
				if (presentSupport) {
					m_QueueHelper.presentqueuefamily = i;
				}

				/**
				* @brief Get compute queue identify.
				*/
				if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
					m_QueueHelper.computequeuefamily = i;
				}

				/**
				* @brief Get transfer queue identify.
				*/
				if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) {
					m_QueueHelper.transferqueuefamily = i;
				}
			}

			if (m_QueueHelper.isComplete()) return true;
		}

		return false;
	}

	SwapChainSupportDetails VulkanDevice::QuerySwapChainSupport(const VkPhysicalDevice& device, const VkSurfaceKHR& surface, GLFWwindow* window)
	{
		/**
		* @brief Get VkSurfaceCapabilitiesKHR.
		*/
		SwapChainSupportDetails swapChainSupportDetails = SwapChainSupportDetails{};
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &swapChainSupportDetails.capabilities);

		/**
		* @brief Get VkExtent2D From VkSurfaceCapabilitiesKHR or glfwWindow.
		*/
		if (swapChainSupportDetails.capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			swapChainSupportDetails.extent = swapChainSupportDetails.capabilities.currentExtent;
		}
		else
		{
			int width, height;
			glfwGetFramebufferSize(window, &width, &height);

			VkExtent2D actualExtent = 
			{
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
			};

			/**
			* @brief clamp to VkSurfaceCapabilitiesKHR's limit.
			*/
			actualExtent.width = std::clamp(actualExtent.width, 
				swapChainSupportDetails.capabilities.minImageExtent.width,
				swapChainSupportDetails.capabilities.maxImageExtent.width);

			actualExtent.height = std::clamp(actualExtent.height, 
				swapChainSupportDetails.capabilities.minImageExtent.height,
				swapChainSupportDetails.capabilities.maxImageExtent.height);

			swapChainSupportDetails.extent = actualExtent;
		}

		/**
		* @brief Get physicaldevice surfaceformat nums;
		*/
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

		if (formatCount != 0) 
		{
			/**
			* @brief  Get physicaldevice surfaceformat;
			*/
			swapChainSupportDetails.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, swapChainSupportDetails.formats.data());

			/**
			* @brief Get suitable surfaceformat.
			* @note Remember this format is linear.
			*/
			swapChainSupportDetails.format = swapChainSupportDetails.formats[0];

			for (const auto& availableFormat : swapChainSupportDetails.formats) 
			{
				if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
					availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) 
				{
					swapChainSupportDetails.format = availableFormat;
					break;
				}
			}
		}

		/**
		* @brief Get physicaldevice persentmode nums;
		*/
		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

		if (presentModeCount != 0)
		{
			/**
			* @brief  Get physicaldevice persentmode;
			*/
			swapChainSupportDetails.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, swapChainSupportDetails.presentModes.data());

			/**
			* @brief Get suitable persentmode.
			*/
			swapChainSupportDetails.presentMode = swapChainSupportDetails.presentModes[0];

			for (const auto& availablePresentMode : swapChainSupportDetails.presentModes) 
			{
				if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) 
				{
					swapChainSupportDetails.presentMode = availablePresentMode;
					break;
				}
			}
		}

		return swapChainSupportDetails;
	}

}