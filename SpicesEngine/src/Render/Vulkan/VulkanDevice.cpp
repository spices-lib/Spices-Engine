/**
* @file VulkanDevice.cpp.
* @brief The VulkanDevice Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "VulkanDevice.h"
#include "Debugger/Perf/NsightPerf.h"

namespace Spices {

	VulkanDevice::VulkanDevice(
		VulkanState&  vulkanState
	)
		: VulkanObject(vulkanState)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Select one suitable physical device.
		*/
		if (!SelectPhysicalDevice(vulkanState.m_Instance, vulkanState.m_Surface, vulkanState.m_Windows)) 
		{
			SPICES_CORE_ERROR("failed select physical device!");
		}

		/**
		* @brief Create a queue identifies container.
		*/
		std::unordered_map<uint32_t, std::unordered_map<uint32_t, std::array<VkQueue, MaxFrameInFlight>>> queueFamilies;  // family - id - queues(flightframes)

		queueFamilies[m_QueueHelper.graphicqueuefamily .value()][0];
		queueFamilies[m_QueueHelper.presentqueuefamily .value()][1];
		queueFamilies[m_QueueHelper.computequeuefamily .value()][2];
		queueFamilies[m_QueueHelper.transferqueuefamily.value()][3];

		/**
		* @brief Fill in VkDeviceQueueCreateInfo.
		*/
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::vector<std::shared_ptr<std::vector<float>>> QueuePriorities;
		for (auto& [family, idItems] : queueFamilies)
		{
			/**
			* @brief Instanced a VkDeviceQueueCreateInfo with default value.
			*/
			VkDeviceQueueCreateInfo                               queueCreateInfo{};
			queueCreateInfo.sType                               = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex                    = family;

			uint32_t queueCount = 0;
			for (auto& pair : idItems)
			{
				queueCount += pair.second.size();
			}

			/**
			* @brief Add a queue for imgui.
			*/
			if (family == m_QueueHelper.graphicqueuefamily.value())
			{
				queueCount++;
			}

			queueCreateInfo.queueCount                          = queueCount;
			
			std::shared_ptr<std::vector<float>> queuePriority   = std::make_shared<std::vector<float>>(queueCount, 1.0f);
			QueuePriorities.push_back(queuePriority);

			queueCreateInfo.pQueuePriorities = queuePriority->data();

			queueCreateInfos.push_back(queueCreateInfo);
		}

		/**
		* @brief Create the feature chain.
		*/
		VkPhysicalDeviceTimelineSemaphoreFeatures                 timelineSemaphore{};
		timelineSemaphore.sType                                 = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES;
		timelineSemaphore.pNext                                 = nullptr;
															    
		VkPhysicalDeviceFragmentShaderBarycentricFeaturesKHR      fragShaderBarycentric{};
		fragShaderBarycentric.sType                             = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADER_BARYCENTRIC_FEATURES_KHR;
		fragShaderBarycentric.pNext                             = &timelineSemaphore;
															    
		VkPhysicalDeviceDiagnosticsConfigFeaturesNV               diagnosticsConfig{};
		diagnosticsConfig.sType                                 = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DIAGNOSTICS_CONFIG_FEATURES_NV;
		diagnosticsConfig.pNext                                 = &fragShaderBarycentric;
												       
		VkPhysicalDeviceDeviceGeneratedCommandsFeaturesNV         dgcFeatures{};
		dgcFeatures.sType                                       = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_GENERATED_COMMANDS_FEATURES_NV;
		dgcFeatures.pNext                                       = &diagnosticsConfig;
														       
		VkPhysicalDeviceVulkan13Features                          vk13Frature{};
		vk13Frature.sType                                       = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
		vk13Frature.pNext                                       = &dgcFeatures;
														       
		VkPhysicalDeviceFragmentShadingRateFeaturesKHR            fragShadingRateFeature {};
		fragShadingRateFeature.sType                            = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_FEATURES_KHR;
		fragShadingRateFeature.pNext                            = &vk13Frature;

		VkPhysicalDeviceMultiviewFeatures                         multiviewFeatures {};
		multiviewFeatures.sType                                 = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES;
		multiviewFeatures.pNext                                 = &fragShadingRateFeature;

		VkPhysicalDeviceMeshShaderFeaturesEXT                     meshShaderFeatures {};
		meshShaderFeatures.sType                                = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_EXT;
		meshShaderFeatures.pNext                                = &multiviewFeatures;
														       
		VkPhysicalDeviceShaderClockFeaturesKHR                    shaderClockFeatures {};
		shaderClockFeatures.sType                               = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CLOCK_FEATURES_KHR;
		shaderClockFeatures.pNext                               = &meshShaderFeatures;
															    
		VkPhysicalDeviceScalarBlockLayoutFeatures                 layoutFeatures {};
		layoutFeatures.sType                                    = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES;
		layoutFeatures.pNext                                    = &shaderClockFeatures;
													            
		VkPhysicalDeviceHostQueryResetFeatures                    hostQueryResetFeatures {};
		hostQueryResetFeatures.sType                            = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_QUERY_RESET_FEATURES;
		hostQueryResetFeatures.pNext                            = &layoutFeatures;

		VkPhysicalDeviceRayTracingPipelineFeaturesKHR             rayTracingFeatures {};
		rayTracingFeatures.sType                                = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
		rayTracingFeatures.pNext                                = &hostQueryResetFeatures; 

		VkPhysicalDeviceAccelerationStructureFeaturesKHR          accelerationStructureFeatures {};
		accelerationStructureFeatures.sType                     = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
		accelerationStructureFeatures.pNext                     = &rayTracingFeatures;

		VkPhysicalDeviceDescriptorIndexingFeatures                descriptorIndexingFeatures {};
		descriptorIndexingFeatures.sType                        = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;
		descriptorIndexingFeatures.pNext                        = &accelerationStructureFeatures;  

		VkPhysicalDeviceBufferDeviceAddressFeatures               bufferDeviceAddressFeatures {};
		bufferDeviceAddressFeatures.sType                       = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES;
		bufferDeviceAddressFeatures.pNext                       = &descriptorIndexingFeatures;

		/**
		* @brief Get all Features that supported.
		*/
		VkPhysicalDeviceFeatures2                                 deviceFeatures{};
		deviceFeatures.sType                                    = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
		deviceFeatures.pNext                                    = &bufferDeviceAddressFeatures;

		/**
		* @brief Fetch all features.
		*/ 
		vkGetPhysicalDeviceFeatures2(m_VulkanState.m_PhysicalDevice, &deviceFeatures);

		/**
		* @brief aftermath config.
		*/
		VkDeviceDiagnosticsConfigCreateInfoNV                     aftermathInfo{};
		aftermathInfo.sType                                     = VK_STRUCTURE_TYPE_DEVICE_DIAGNOSTICS_CONFIG_CREATE_INFO_NV;
		aftermathInfo.flags                                     = VK_DEVICE_DIAGNOSTICS_CONFIG_ENABLE_AUTOMATIC_CHECKPOINTS_BIT_NV  |  // Enable automatic call stack checkpoints.
											                      VK_DEVICE_DIAGNOSTICS_CONFIG_ENABLE_RESOURCE_TRACKING_BIT_NV      |  // Enable tracking of resources.
											                      VK_DEVICE_DIAGNOSTICS_CONFIG_ENABLE_SHADER_DEBUG_INFO_BIT_NV      |  // Generate debug information for shaders.
							            	                      VK_DEVICE_DIAGNOSTICS_CONFIG_ENABLE_SHADER_ERROR_REPORTING_BIT_NV ;  // Enable additional runtime shader error reporting.
		aftermathInfo.pNext                                     = &deviceFeatures;

		/**
		* @brief Instanced a VkDeviceCreateInfo with default value.
		*/
		VkDeviceCreateInfo                                        createInfo{};
		createInfo.sType                                        = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos                            = queueCreateInfos.data();
		createInfo.queueCreateInfoCount                         = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pEnabledFeatures                             = VK_NULL_HANDLE;
		createInfo.enabledExtensionCount                        = static_cast<uint32_t>(m_ExtensionProperties.size());
		createInfo.ppEnabledExtensionNames                      = m_ExtensionProperties.data();
		createInfo.enabledLayerCount                            = 0;
		createInfo.pNext                                        = &aftermathInfo;

		/**
		* @brief Create device and set it global.
		*/
		VK_CHECK(vkCreateDevice(vulkanState.m_PhysicalDevice, &createInfo, nullptr, &vulkanState.m_Device));
		DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_DEVICE, (uint64_t)vulkanState.m_Device, vulkanState.m_Device, m_DeviceProperties.deviceName)

		/**
		* @brief Get Queues.
		*/
		{
			for (auto& [ family, idItems] : queueFamilies)
			{
				int index = 0;
				for (auto& [id, items] : idItems)
				{
					for (int i = 0; i < MaxFrameInFlight; i++)
					{
						vkGetDeviceQueue(vulkanState.m_Device, family, index, &queueFamilies[family][id][i]);
						index++;
					}
				}

				if (family == m_QueueHelper.graphicqueuefamily.value())
				{
					vkGetDeviceQueue(vulkanState.m_Device, family, index, &vulkanState.m_SlateGraphicQueue);
				}
			}

			vulkanState.m_GraphicQueues  = queueFamilies[m_QueueHelper.graphicqueuefamily .value()][0];
			vulkanState.m_PresentQueues  = queueFamilies[m_QueueHelper.presentqueuefamily .value()][1];
			vulkanState.m_ComputeQueues  = queueFamilies[m_QueueHelper.computequeuefamily .value()][2];
			vulkanState.m_TransferQueues = queueFamilies[m_QueueHelper.transferqueuefamily.value()][3];

			for(int i = 0; i < MaxFrameInFlight; i++)
			{
				DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_QUEUE, (uint64_t)vulkanState.m_GraphicQueues[i] , vulkanState.m_Device, "GraphicQueue" );
				DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_QUEUE, (uint64_t)vulkanState.m_PresentQueues[i] , vulkanState.m_Device, "PresentQueue" );
				DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_QUEUE, (uint64_t)vulkanState.m_ComputeQueues[i] , vulkanState.m_Device, "ComputeQueue" );
				DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_QUEUE, (uint64_t)vulkanState.m_TransferQueues[i], vulkanState.m_Device, "TransferQueue");
			}

			DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_QUEUE, (uint64_t)vulkanState.m_SlateGraphicQueue, vulkanState.m_Device, "SlateGraphicQueue");
		}
	}

	VulkanDevice::~VulkanDevice()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Destroy the Vulkan Device Object.
		* Queue is created by device, we do not need destroy queue here manually.
		*/
		vkDestroyDevice(m_VulkanState.m_Device, nullptr);
	}

	void VulkanDevice::RequerySwapChainSupport()
	{
		SPICES_PROFILE_ZONE;

		m_SwapChainSupportDetails = 
			QuerySwapChainSupport(
			m_VulkanState.m_PhysicalDevice, 
			m_VulkanState.m_Surface, m_VulkanState.m_Windows
		);
	}

	VkSampleCountFlagBits VulkanDevice::GetMaxUsableSampleCount() const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Get VkPhysicalDeviceProperties.
		*/
		VkPhysicalDeviceProperties physicalDeviceProperties;
		vkGetPhysicalDeviceProperties(m_VulkanState.m_PhysicalDevice, &physicalDeviceProperties);

		const VkSampleCountFlags counts = 
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
		SPICES_PROFILE_ZONE;

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
			SPICES_CORE_WARN("Failed to find GPUs with Vulkan support!");
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

				SPICES_CORE_INFO("VkPhysicalDevice Selected!!!");
				return true;
			}
		}

		return false;
	}

	bool VulkanDevice::IsPropertyMeetDemand(const VkPhysicalDevice& device)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Get all RayTracing Properties supported.
		*/
		m_DGCProperties.sType                       = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_GENERATED_COMMANDS_PROPERTIES_NV;
		m_DGCProperties.pNext                       = nullptr;

		m_RayTracingProperties.sType                = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR;
		m_RayTracingProperties.pNext                = &m_DGCProperties;

		VkPhysicalDeviceMeshShaderPropertiesEXT       meshShaderProperties{};
		meshShaderProperties.sType                  = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_PROPERTIES_EXT;
		meshShaderProperties.pNext                  = &m_RayTracingProperties;
		
		VkPhysicalDeviceSubgroupProperties            subGroupProperties{};
		subGroupProperties.sType                    = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_PROPERTIES;
		subGroupProperties.pNext                    = &meshShaderProperties;

		VkPhysicalDeviceProperties2                   prop2 {};
		prop2.sType                                 = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
		prop2.pNext                                 = &subGroupProperties;
		vkGetPhysicalDeviceProperties2(device, &prop2);
		m_DeviceProperties                          = prop2.properties;

		{
			std::stringstream ss;
			ss << "Limits : maxFragmentOutputAttachments = " << m_DeviceProperties.limits.maxFragmentOutputAttachments;
			SPICES_CORE_INFO(ss.str());
		}

		{
			std::stringstream ss;
			ss << "MeshShader : maxMeshOutputVertices = " << meshShaderProperties.maxMeshOutputVertices;
			SPICES_CORE_INFO(ss.str());
		}

		{
			std::stringstream ss;
			ss << "MeshShader : maxMeshOutputPrimitives = " << meshShaderProperties.maxMeshOutputPrimitives;
			SPICES_CORE_INFO(ss.str());
		}

		{
			std::stringstream ss;
			ss << "SubGroup : subgroupSize = " << subGroupProperties.subgroupSize;
			SPICES_CORE_INFO(ss.str());
			assert(subGroupProperties.supportedStages & VK_SHADER_STAGE_TASK_BIT_EXT);
			assert(subGroupProperties.supportedStages & VK_SHADER_STAGE_MESH_BIT_EXT);
		}

		return true;
	}

	bool VulkanDevice::IsFeatureMeetDemand(const VkPhysicalDevice& device)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Create the feature chain.
		*/
		VkPhysicalDeviceTimelineSemaphoreFeatures             timelineSemaphore{};
		timelineSemaphore.sType                             = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES;
		timelineSemaphore.pNext                             = nullptr;

		VkPhysicalDeviceFragmentShaderBarycentricFeaturesKHR  fragShaderBarycentric{};
		fragShaderBarycentric.sType                         = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADER_BARYCENTRIC_FEATURES_KHR;
		fragShaderBarycentric.pNext                         = &timelineSemaphore;

		VkPhysicalDeviceDiagnosticsConfigFeaturesNV           diagnosticsConfig{};
		diagnosticsConfig.sType                             = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DIAGNOSTICS_CONFIG_FEATURES_NV;
		diagnosticsConfig.pNext                             = &fragShaderBarycentric;

		VkPhysicalDeviceDeviceGeneratedCommandsFeaturesNV     dgcFeatures{};
		dgcFeatures.sType                                   = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_GENERATED_COMMANDS_FEATURES_NV;
		dgcFeatures.pNext                                   = &diagnosticsConfig;

		VkPhysicalDeviceVulkan13Features                      vk13Frature{};
		vk13Frature.sType                                   = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
		vk13Frature.pNext                                   = &dgcFeatures;

		VkPhysicalDeviceFragmentShadingRateFeaturesKHR        fragShadingRateFeature {};
		fragShadingRateFeature.sType                        = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_FEATURES_KHR;
		fragShadingRateFeature.pNext                        = &vk13Frature;

		VkPhysicalDeviceMultiviewFeatures                     multiviewFeatures {};
		multiviewFeatures.sType                             = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES;
		multiviewFeatures.pNext                             = &fragShadingRateFeature;

		VkPhysicalDeviceMeshShaderFeaturesEXT                 meshShaderFeatures {};
		meshShaderFeatures.sType                            = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_EXT;
		meshShaderFeatures.pNext                            = &multiviewFeatures;
		
		VkPhysicalDeviceShaderClockFeaturesKHR                shaderClockFeatures{};
		shaderClockFeatures.sType                           = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CLOCK_FEATURES_KHR;
		shaderClockFeatures.pNext                           = &meshShaderFeatures;

		VkPhysicalDeviceScalarBlockLayoutFeatures             layoutFeatures {};
		layoutFeatures.sType                                = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES;
		layoutFeatures.pNext                                = &shaderClockFeatures;
													        
		VkPhysicalDeviceHostQueryResetFeatures                hostQueryResetFeatures {};
		hostQueryResetFeatures.sType                        = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_QUERY_RESET_FEATURES;
		hostQueryResetFeatures.pNext                        = &layoutFeatures;
													        
		VkPhysicalDeviceRayTracingPipelineFeaturesKHR         rayTracingFeatures {};
		rayTracingFeatures.sType                            = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
		rayTracingFeatures.pNext                            = &hostQueryResetFeatures; 

		VkPhysicalDeviceAccelerationStructureFeaturesKHR      accelerationStructureFeatures {};
		accelerationStructureFeatures.sType                 = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
		accelerationStructureFeatures.pNext                 = &rayTracingFeatures; 
													       
		VkPhysicalDeviceDescriptorIndexingFeatures            descriptorIndexingFeatures {};
		descriptorIndexingFeatures.sType                    = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;
		descriptorIndexingFeatures.pNext                    = &accelerationStructureFeatures;
													       
		VkPhysicalDeviceBufferDeviceAddressFeatures           bufferDeviceAddressFeatures {};
		bufferDeviceAddressFeatures.sType                   = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES;
		bufferDeviceAddressFeatures.pNext                   = &descriptorIndexingFeatures;

		/**
		* @brief Get all Features that supported.
		*/
		VkPhysicalDeviceFeatures2 deviceFeatures {};
		deviceFeatures.sType                                = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
		deviceFeatures.pNext                                = &bufferDeviceAddressFeatures;

		/**
		* @Fetch all features.
		*/ 
		vkGetPhysicalDeviceFeatures2(device, &deviceFeatures);

		/**
		* @brief Just return true for we do not need a specific feature supported now.
		* @todo Configurable.
		*/
		ASSERT(deviceFeatures.features.samplerAnisotropy)                                     /* @brief Enable samplerAnisotropy.                       */
		ASSERT(deviceFeatures.features.sampleRateShading)                                     /* @brief Enable sampleRateShading, for MSAA.             */
		ASSERT(deviceFeatures.features.independentBlend)                                      /* @brief Enable Independent Attachment AlphaBlend State. */
		ASSERT(deviceFeatures.features.geometryShader)                                        /* @brief Enable Geometry Shader Feature.                 */
		ASSERT(bufferDeviceAddressFeatures.bufferDeviceAddress)                               /* @brief Enable Buffer Address Feature.                  */
		ASSERT(accelerationStructureFeatures.accelerationStructure)                           /* @brief Enable RayTracing AccelerationStructure.        */
		ASSERT(rayTracingFeatures.rayTracingPipeline)                                         /* @brief Enable RayTracing Pipeline.                     */
		ASSERT(hostQueryResetFeatures.hostQueryReset)                                         /* @brief Enable HostQueryReset Feature.                  */
		ASSERT(layoutFeatures.scalarBlockLayout)                                              /* @brief Enable Shader ScalarBlockLayout Feature.        */

		ASSERT(descriptorIndexingFeatures.shaderSampledImageArrayNonUniformIndexing)          /* @brief Enable Shader shaderSampledImageArrayNonUniformIndexing Feature.        */
		ASSERT(descriptorIndexingFeatures.descriptorBindingSampledImageUpdateAfterBind)       /* @brief Enable Shader descriptorBindingSampledImageUpdateAfterBind Feature.     */
		ASSERT(descriptorIndexingFeatures.shaderUniformBufferArrayNonUniformIndexing)         /* @brief Enable Shader shaderUniformBufferArrayNonUniformIndexing Feature.       */
		ASSERT(descriptorIndexingFeatures.descriptorBindingUniformBufferUpdateAfterBind)      /* @brief Enable Shader descriptorBindingUniformBufferUpdateAfterBind Feature.    */
		ASSERT(descriptorIndexingFeatures.shaderStorageBufferArrayNonUniformIndexing)         /* @brief Enable Shader shaderStorageBufferArrayNonUniformIndexing Feature.       */
		ASSERT(descriptorIndexingFeatures.descriptorBindingStorageBufferUpdateAfterBind)      /* @brief Enable Shader descriptorBindingStorageBufferUpdateAfterBind Feature.    */
		ASSERT(descriptorIndexingFeatures.descriptorBindingPartiallyBound)                    /* @brief Enable descriptorBindingPartiallyBound Feature.                         */
		ASSERT(descriptorIndexingFeatures.runtimeDescriptorArray)                             /* @brief Enable runtimeDescriptorArray Feature.                                  */

		ASSERT(shaderClockFeatures.shaderSubgroupClock)                                       /* @brief Enable Shader clock time Feature.                                       */
		
		ASSERT(meshShaderFeatures.meshShader)                                                 /* @brief Enable Mesh Shader Feature.                                       */
		ASSERT(meshShaderFeatures.taskShader)                                                 /* @brief Enable Task Shader Feature.                                       */

		ASSERT(multiviewFeatures.multiview)                                                   /* @brief Enable Multiview Feature.                                         */
		ASSERT(fragShadingRateFeature.pipelineFragmentShadingRate)                            /* @brief Enable pipelineFragmentShadingRate Feature.                       */
		ASSERT(vk13Frature.maintenance4)                                                      /* @brief Enable maintenance4 Feature.                                      */

		ASSERT(dgcFeatures.deviceGeneratedCommands)                                            /* @brief Enable Nvidia GPU Generate Commands Feature.                     */

		ASSERT(diagnosticsConfig.diagnosticsConfig)                                            /* @brief Enable Nvidia GPU Generate Diagnostic Checkpoints Feature.       */
		
		ASSERT(fragShaderBarycentric.fragmentShaderBarycentric)                                /* @brief Enable FragmentShader Barycentric access Feature.                */

		ASSERT(timelineSemaphore.timelineSemaphore)                                            /* @brief Enable timeline semaphore Feature.                               */

		return true;
	}

	void VulkanDevice::GetExtensionRequirements()
	{
		SPICES_PROFILE_ZONE;

		m_ExtensionProperties.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);                        /* @brief Swapchain Extension.                      */
		m_ExtensionProperties.push_back(VK_KHR_MAINTENANCE_1_EXTENSION_NAME);                    /* @brief Negative Viewport Extension.              */
		m_ExtensionProperties.push_back(VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME);           /* @brief To build acceleration structures.         */
		m_ExtensionProperties.push_back(VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME);             /* @brief To use vkCmdTraceRaysKHR.                 */
		m_ExtensionProperties.push_back(VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME);         /* @brief Required by ray tracing pipeline.         */
		m_ExtensionProperties.push_back(VK_KHR_SHADER_CLOCK_EXTENSION_NAME);                     /* @brief Enable Shader Clock Extension.            */
		m_ExtensionProperties.push_back(VK_KHR_SHADER_NON_SEMANTIC_INFO_EXTENSION_NAME);         /* @brief Enable Shader Debug Print.                */
		m_ExtensionProperties.push_back(VK_KHR_SPIRV_1_4_EXTENSION_NAME);                        /* @brief Enable Shader spirv1.4.                   */
		m_ExtensionProperties.push_back(VK_EXT_MESH_SHADER_EXTENSION_NAME);                      /* @brief Enable Mesh Shader, Task Shader.          */
		m_ExtensionProperties.push_back(VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME);            /* @brief Enable Shader float controls.             */
		m_ExtensionProperties.push_back(VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME);            /* @brief Enable Fragment Shadeing rate.            */
		m_ExtensionProperties.push_back(VK_EXT_NESTED_COMMAND_BUFFER_EXTENSION_NAME);            /* @brief Enable Nested Command Buffer.             */
		m_ExtensionProperties.push_back(VK_NV_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME);         /* @brief Enable Nvidia GPU Generate Commands.      */
		m_ExtensionProperties.push_back(VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME);     /* @brief Enable Nvidia GPU Diagnostic Checkpoints. */
		m_ExtensionProperties.push_back(VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME);         /* @brief Enable Nvidia GPU Diagnostic Config.      */
		m_ExtensionProperties.push_back(VK_KHR_FRAGMENT_SHADER_BARYCENTRIC_EXTENSION_NAME);      /* @brief Enable FragmentShaderBarycentric.         */

		/**
		* @brief Those Extensions are enabled other place.
		* 
		* VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME                                                  @brief Enable timeline semaphore.                
		*/            
	}

	bool VulkanDevice::IsExtensionMeetDemand(const VkPhysicalDevice& device)
	{
		SPICES_PROFILE_ZONE;

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

		/**
		* @brief Get NvperfDeviceExtensions.
		*/
		std::vector<const char*> nvperfDeviceExtensions;
		//PERF_QUERYDEVICEEXTENSION(m_VulkanState.m_Instance, device, nvperfDeviceExtensions)

		for (auto& e : nvperfDeviceExtensions)
		{
			requiredExtensions.insert(e);
		}

		for (const auto& extension : availableExtensions) 
		{
			requiredExtensions.erase(extension.extensionName);
		}

		if (requiredExtensions.empty())
		{
			/**
			* @brief Add NvperfDeviceExtensions to m_ExtensionProperties.
			*/
			m_ExtensionProperties.insert(m_ExtensionProperties.end(), nvperfDeviceExtensions.begin(), nvperfDeviceExtensions.end());

			return true;
		}
		else
		{
			for (auto& set : requiredExtensions)
			{
				VkPhysicalDeviceProperties deviceProperties;
				vkGetPhysicalDeviceProperties(device, &deviceProperties);

				std::stringstream ss;
				ss << "Device Extension Required: " << set << ", Which is not satisfied with device: " << deviceProperties.deviceName;
				SPICES_CORE_WARN(ss.str());
			}

			return false;
		}
	}

	bool VulkanDevice::IsQueueMeetDemand(const VkPhysicalDevice& device, const VkSurfaceKHR& surface)
	{
		SPICES_PROFILE_ZONE;

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
			const auto& queueFamily = queueFamilies[i];

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
					m_VulkanState.m_ComputeQueueFamily = i;
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
		SPICES_PROFILE_ZONE;

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