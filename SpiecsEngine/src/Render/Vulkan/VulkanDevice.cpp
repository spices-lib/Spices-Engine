/**
* @file VulkanDevice.cpp.
* @brief The VulkanDevice Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "VulkanDevice.h"

namespace Spiecs {

	VulkanDevice::VulkanDevice(
		VulkanState&  vulkanState
	)
		: VulkanObject(vulkanState)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Select one suitable physical device.
		*/
		if (!SelectPhysicalDevice(vulkanState.m_Instance, vulkanState.m_Surface, vulkanState.m_Windows)) 
		{
			SPIECS_CORE_INFO("failed select physical device!");
		}

		/**
		* @brief Create a queue identifies container.
		*/
		std::set<uint32_t> uniqueQueueFamilies = 
		{   
			m_QueueHelper.graphicqueuefamily.value(), 
			m_QueueHelper.presentqueuefamily.value(), 
			m_QueueHelper.computequeuefamily.value(),
			m_QueueHelper.transferqueuefamily.value() 
		};

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
			queueCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount       = 1;

			float queuePriority = 1.0f;
			queueCreateInfo.pQueuePriorities = &queuePriority;

			queueCreateInfos.push_back(queueCreateInfo);
		}

		/**
		* @brief Create the feature chain.
		*/
		VkPhysicalDeviceHostQueryResetFeatures hostQueryResetFeatures {};
		hostQueryResetFeatures.sType                            = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_QUERY_RESET_FEATURES;
		hostQueryResetFeatures.pNext                            = nullptr;  /*@brief Pass your other features through this chain.*/

		VkPhysicalDeviceRayTracingPipelineFeaturesKHR rayTracingFeatures {};
		rayTracingFeatures.sType                                = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
		rayTracingFeatures.pNext                                = &hostQueryResetFeatures; 

		VkPhysicalDeviceAccelerationStructureFeaturesKHR accelerationStructureFeatures{};
		accelerationStructureFeatures.sType                     = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
		accelerationStructureFeatures.pNext                     = &rayTracingFeatures;

		VkPhysicalDeviceDescriptorIndexingFeatures descriptorIndexingFeatures{};
		descriptorIndexingFeatures.sType                        = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;
		descriptorIndexingFeatures.pNext                        = &accelerationStructureFeatures;  

		VkPhysicalDeviceBufferDeviceAddressFeatures bufferDeviceAddressFeatures{};
		bufferDeviceAddressFeatures.sType                       = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES;
		bufferDeviceAddressFeatures.pNext                       = &descriptorIndexingFeatures;

		/**
		* @brief Get all Features that supported.
		*/
		VkPhysicalDeviceFeatures2 deviceFeatures{};
		deviceFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
		deviceFeatures.pNext = &bufferDeviceAddressFeatures;

		// Fetch all features
		vkGetPhysicalDeviceFeatures2(m_VulkanState.m_PhysicalDevice, &deviceFeatures);

		/**
		* @brief Instanced a VkDeviceCreateInfo with default value.
		*/
		VkDeviceCreateInfo createInfo{};
		createInfo.sType                    = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos        = queueCreateInfos.data();
		createInfo.queueCreateInfoCount     = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pEnabledFeatures         = VK_NULL_HANDLE;
		createInfo.enabledExtensionCount    = static_cast<uint32_t>(m_ExtensionProperties.size());
		createInfo.ppEnabledExtensionNames  = m_ExtensionProperties.data();
		createInfo.enabledLayerCount        = 0;
		createInfo.pNext                    = &deviceFeatures;

		/**
		* @brief Create device and set it global.
		*/
		VK_CHECK(vkCreateDevice(vulkanState.m_PhysicalDevice, &createInfo, nullptr, &vulkanState.m_Device));
		VulkanDebugUtils::SetObjectName(VK_OBJECT_TYPE_DEVICE, vulkanState.m_Device, vulkanState.m_Device, m_DeviceProperties.deviceName);

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
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Destroy the Vulkan Device Object.
		* Queue is created by device, we do not need destroy queue here manually.
		*/
		vkDestroyDevice(m_VulkanState.m_Device, nullptr);
	}

	void VulkanDevice::RequerySwapChainSupport()
	{
		SPIECS_PROFILE_ZONE;

		m_SwapChainSupportDetails = 
			QuerySwapChainSupport(
			m_VulkanState.m_PhysicalDevice, 
			m_VulkanState.m_Surface, m_VulkanState.m_Windows
		);
	}

	VkSampleCountFlagBits VulkanDevice::GetMaxUsableSampleCount()
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Get VkPhysicalDeviceProperties.
		*/
		VkPhysicalDeviceProperties physicalDeviceProperties;
		vkGetPhysicalDeviceProperties(m_VulkanState.m_PhysicalDevice, &physicalDeviceProperties);

		VkSampleCountFlags counts = 
			physicalDeviceProperties.limits.framebufferColorSampleCounts & 
			physicalDeviceProperties.limits.framebufferDepthSampleCounts;

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
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Get all physical device num this computer.
		*/
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

		/**
		* @brief This software requires there must be at least one physical device.
		*/
		if (deviceCount == 0) 
		{
			SPIECS_CORE_WARN("Failed to find GPUs with Vulkan support!");
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
			if (IsExtensionMeetDemand(physicalDevice) && 
				IsPropertyMeetDemand(physicalDevice) &&
				IsFeatureMeetDemand(physicalDevice) && 
				IsQueueMeetDemand(physicalDevice, surface)
				)
			{
				/**
				* @brief Set Selected physical device.
				*/
				m_VulkanState.m_PhysicalDevice = physicalDevice;

				/**
				* @brief Query physicaldevice's SwapChainSupport.
				*/
				m_SwapChainSupportDetails = QuerySwapChainSupport(physicalDevice, surface, window);

				SPIECS_CORE_INFO("VkPhysicalDevice Selected!!!");
				return true;
			}
		}

		return false;
	}

	bool VulkanDevice::IsPropertyMeetDemand(const VkPhysicalDevice& device)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Get all Properties that supported.
		*/
		vkGetPhysicalDeviceProperties(device, &m_DeviceProperties);

		/**
		* @brief Get all RayTracing Properties supported.
		*/
		VkPhysicalDeviceProperties2 prop2{};
		prop2.sType                      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
		prop2.pNext                      = &m_RayTracingProperties;
		prop2.properties                 = m_DeviceProperties;
		vkGetPhysicalDeviceProperties2(device, &prop2);

		return true;
	}

	bool VulkanDevice::IsFeatureMeetDemand(const VkPhysicalDevice& device)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Create the feature chain.
		*/ 
		VkPhysicalDeviceHostQueryResetFeatures hostQueryResetFeatures {};
		hostQueryResetFeatures.sType                  = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_QUERY_RESET_FEATURES;
		hostQueryResetFeatures.pNext                  = nullptr;/*@brief Pass your other features through this chain.*/

		VkPhysicalDeviceRayTracingPipelineFeaturesKHR rayTracingFeatures {};
		rayTracingFeatures.sType                      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
		rayTracingFeatures.pNext                      = &hostQueryResetFeatures; 

		VkPhysicalDeviceAccelerationStructureFeaturesKHR accelerationStructureFeatures{};
		accelerationStructureFeatures.sType           = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
		accelerationStructureFeatures.pNext           = &rayTracingFeatures; 

		VkPhysicalDeviceDescriptorIndexingFeatures descriptorIndexingFeatures {};
		descriptorIndexingFeatures.sType              = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;
		descriptorIndexingFeatures.pNext              = &accelerationStructureFeatures;  /*@brief Pass your other features through this chain.*/

		VkPhysicalDeviceBufferDeviceAddressFeatures bufferDeviceAddressFeatures {};
		bufferDeviceAddressFeatures.sType             = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES;
		bufferDeviceAddressFeatures.pNext             = &descriptorIndexingFeatures;

		/**
		* @brief Get all Features that supported.
		*/
		VkPhysicalDeviceFeatures2 deviceFeatures {};
		deviceFeatures.sType                          = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
		deviceFeatures.pNext                          = &bufferDeviceAddressFeatures;

		// Fetch all features
		vkGetPhysicalDeviceFeatures2(device, &deviceFeatures);

		/**
		* @brief Just return true for we do not need a specific feature supported now.
		* @todo Configurable.
		*/
		if (!deviceFeatures.features.samplerAnisotropy) return false;                  /*@brief 启用纹理采样的各项异性*/
		if (!deviceFeatures.features.sampleRateShading) return false;                  /*@brief 启用实例着色（应对MSAA的缺点）*/
		if (!deviceFeatures.features.independentBlend) return false;                   /*@breif Enable Independent Attachment AlphaBlend State.*/
		if (!deviceFeatures.features.geometryShader) return false;                     /*@breif Enable Geometry Shader Feature.*/
		if (!bufferDeviceAddressFeatures.bufferDeviceAddress) return false;            /*@breif Enable Buffer Address Feature.*/
		if (!accelerationStructureFeatures.accelerationStructure) return false;        /*@breif Enable RayTracing AccelerationStructure.*/
		if (!rayTracingFeatures.rayTracingPipeline) return false;                      /*@breif Enable RayTracing Pipeline.*/
		if (!hostQueryResetFeatures.hostQueryReset) return false;                      /*@brief Enable HostQueryReset Feature.*/

		return true;
	}

	void VulkanDevice::GetExtensionRequirements()
	{
		SPIECS_PROFILE_ZONE;

		m_ExtensionProperties.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);                        /*@brief Swapchain Extension.*/
																						         
		m_ExtensionProperties.push_back(VK_KHR_MAINTENANCE_1_EXTENSION_NAME);                    /*@brief Negative Viewpoet Extension.*/
																						         
		m_ExtensionProperties.push_back(VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME);           /*@brief To build acceleration structures.*/
		m_ExtensionProperties.push_back(VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME);             /*To use vkCmdTraceRaysKHR.*/
		m_ExtensionProperties.push_back(VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME);         /*@brief Required by ray tracing pipeline*/
	}

	bool VulkanDevice::IsExtensionMeetDemand(const VkPhysicalDevice& device)
	{
		SPIECS_PROFILE_ZONE;

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

		for (const auto& extension : availableExtensions) 
		{
			requiredExtensions.erase(extension.extensionName);
		}

		if (!requiredExtensions.empty())
		{
			for (auto& set : requiredExtensions)
			{
				VkPhysicalDeviceProperties deviceProperties;
				vkGetPhysicalDeviceProperties(device, &deviceProperties);

				std::stringstream ss;
				ss << "Device Extension Required: " << set << ", Which is not satisfied with device: " << deviceProperties.deviceName;
				SPIECS_CORE_WARN(ss.str());
			}
		}

		return requiredExtensions.empty();
	}

	bool VulkanDevice::IsQueueMeetDemand(const VkPhysicalDevice& device, const VkSurfaceKHR& surface)
	{
		SPIECS_PROFILE_ZONE;

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
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
			{
				m_QueueHelper.graphicqueuefamily = i;
				m_VulkanState.m_GraphicQueueFamily = i;

				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

				/**
				* @brief Get present queue identify.
				*/
				if (presentSupport) 
				{
					m_QueueHelper.presentqueuefamily = i;
				}

				/**
				* @brief Get compute queue identify.
				*/
				if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) 
				{
					m_QueueHelper.computequeuefamily = i;
				}

				/**
				* @brief Get transfer queue identify.
				*/
				if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) 
				{
					m_QueueHelper.transferqueuefamily = i;
				}
			}

			if (m_QueueHelper.isComplete()) return true;
		}

		return false;
	}

	SwapChainSupportDetails VulkanDevice::QuerySwapChainSupport(
		const VkPhysicalDevice& device  , 
		const VkSurfaceKHR&     surface , 
		GLFWwindow*             window
	)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Get VkSurfaceCapabilitiesKHR.
		*/
		SwapChainSupportDetails swapChainSupportDetails = SwapChainSupportDetails{};
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &swapChainSupportDetails.capabilities);

		/**
		* @brief Get VkExtent2D From VkSurfaceCapabilitiesKHR or glfwWindow.
		*/
		if (swapChainSupportDetails.capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) 
		{
			swapChainSupportDetails.surfaceSize = swapChainSupportDetails.capabilities.currentExtent;
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

			swapChainSupportDetails.surfaceSize = actualExtent;
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