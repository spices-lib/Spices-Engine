/**
* @file VulkanInstance.cpp.
* @brief The VulkanInstance Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "VulkanInstance.h"
#include "Core/Library/StringLibrary.h"
#include "Debugger/Perf/NsightPerf.h"

namespace Spices {

	VulkanInstance::VulkanInstance(
		VulkanState&       vulkanState , 
		const std::string& name        , 
		const std::string& engineName
	)
		: VulkanObject(vulkanState)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Create VkApplicationInfo struct.
		*/
		VkApplicationInfo               appInfo {};
		appInfo.sType                 = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName      = name.c_str();
		appInfo.applicationVersion    = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName           = engineName.c_str();
		appInfo.engineVersion         = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion            = VK_API_VERSION_1_3;

		/**
		* @brief Create VkInstanceCreateInfo struct.
		*/
		VkInstanceCreateInfo            createInfo {};
		createInfo.sType              = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo   = &appInfo;

		/**
		* @brief Get all instance extension requirements our engine needede.
		*/
		GetExtensionRequirements();

		/**
		* @brief Iter all our extensions, check whether all satisfied or not.
		*/
		if (!CheckExtensionRequirementsSatisfied())
		{
			std::stringstream ss;
			ss << "Instance Extension not Satisfied";

			SPICES_CORE_ERROR(ss.str());
		}

		/**
		* @brief Set instance extension.
		*/
		createInfo.enabledExtensionCount         = static_cast<uint32_t>(m_ExtensionProperties.size());
		createInfo.ppEnabledExtensionNames       = m_ExtensionProperties.data();

		/**
		* @brief Get all instance layer requirements our engine needede.
		*/
		GetLayerRequirements();

		/**
		* @brief Iter all our layers, check whether all satisfied or not.
		*/
		if (!ChecklayerRequirementsSatisfied()) return;

		/**
		* @brief Set instance layer.
		*/
		createInfo.enabledLayerCount             = static_cast<uint32_t>(m_LayerProperties.size());
		createInfo.ppEnabledLayerNames           = m_LayerProperties.data();

		/**
		* @brief Set VkDebugUtilsMessengerCreateInfoEXT.
		*/
		FillDebugMessengerCreateInfo();

		/**
		* @brief Enable Shader Debug Feature.
		*/
		std::vector<VkValidationFeatureEnableEXT> validationFeatureEnable;
		validationFeatureEnable.push_back(VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT);
		
		VkValidationFeaturesEXT                               validationFeatures{};
		validationFeatures.sType                            = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
		validationFeatures.enabledValidationFeatureCount    = static_cast<uint32_t>(validationFeatureEnable.size());
		validationFeatures.pEnabledValidationFeatures       = validationFeatureEnable.data();

#ifdef SPICES_DEBUG

		m_DebugMessengerCreateInfo.pNext                    = &validationFeatures;

#endif

		createInfo.pNext = &m_DebugMessengerCreateInfo;

		/**
		* @brief Create instance and set it global.
		*/
		VK_CHECK(vkCreateInstance(&createInfo, nullptr, &vulkanState.m_Instance))

		/**
		* @brief Init Vulkan Functions.
		*/
		vulkanState.m_VkFunc.Init(vulkanState.m_Instance);

		/**
		* @brief Set Vulkan's debug message callback function pointer.
		*/
		SetVulkanDebugCallbackFuncPointer();

		/**
		* @brief Create Surface.
		*/
		CreateVulkanSurface();
	}

	VulkanInstance::~VulkanInstance()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Destroy the Vulkan Surface Object.
		*/
		vkDestroySurfaceKHR(m_VulkanState.m_Instance, m_VulkanState.m_Surface, nullptr);

		/**
		* @brief Destroy Vulkan's debug message callback function pointer.
		* Working with DEBUG mode.
		*/
#ifdef SPICES_DEBUG

		/**
		* @brief Destroy Debug Utils Messenger.
		*/
		m_VulkanState.m_VkFunc.vkDestroyDebugUtilsMessengerEXT(m_VulkanState.m_Instance, m_DebugMessenger, nullptr);

#endif

		/**
		* @brief Destroy the Vulkan Instance Object.
		*/
		vkDestroyInstance(m_VulkanState.m_Instance, nullptr);
	}

	void VulkanInstance::GetExtensionRequirements()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Get glfw extensions requirements.
		*/
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		/**
		* @brief Combine glfw extensions requirements.
		*/
		for (uint32_t i = 0; i < glfwExtensionCount; i++) 
		{
			m_ExtensionProperties.push_back(*glfwExtensions);
			glfwExtensions++;
		}

		/**
		* @brief Combine with our extensions requirements.
		* @todo Our instance extensions requirements.
		*/
#ifdef SPICES_DEBUG

		/**
		* @brief To enable validation layer, we need this instance extension.
		*/
		m_ExtensionProperties.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		/**
		* @brief Add Nsight Perf Extensions.
		*/
		PERF_QUERYINSTANCEEXTENSION(m_ExtensionProperties, VK_API_VERSION_1_3);

#endif
	}

	bool VulkanInstance::CheckExtensionRequirementsSatisfied()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Get all instance extensions nums.
		*/
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

		/**
		* @brief Get all instance extensions that supported.
		*/
		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());
		
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
				std::stringstream ss;
				ss << "Instance Extension Required: " << set << ", Which is not satisfied";
				
				SPICES_CORE_WARN(ss.str());
			}
		}

		return requiredExtensions.empty();
	}

	void VulkanInstance::GetLayerRequirements()
	{
		SPICES_PROFILE_ZONE;

#ifdef SPICES_DEBUG

		/**
		* @brief Combine with our extensions requirements.
		*/
		m_LayerProperties.push_back("VK_LAYER_KHRONOS_validation");

#endif

	}

	bool VulkanInstance::ChecklayerRequirementsSatisfied()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Get all layer extensions nums.
		*/
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		/**
		* @brief Get all instance layer that supported.
		*/
		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		/**
		* @brief Check whether all layer satisfied.
		*/
		std::set<std::string> requiredLayers(m_LayerProperties.begin(), m_LayerProperties.end());

		for (const auto& layer : availableLayers) 
		{
			requiredLayers.erase(layer.layerName);
		}

		if (!requiredLayers.empty())
		{
			for (auto& set : requiredLayers)
			{
				std::stringstream ss;
				ss << "Instance Layer Required: " << set << ", Which is not satisfied";
				
				SPICES_CORE_WARN(ss.str());
			}
		}

		return requiredLayers.empty();
	}

	void VulkanInstance::SetVulkanDebugCallbackFuncPointer()
	{
		SPICES_PROFILE_ZONE;

#ifdef SPICES_DEBUG

		/**
		* @brief Create DebugUtilsMessenger.
		*/
		m_VulkanState.m_VkFunc.vkCreateDebugUtilsMessengerEXT(m_VulkanState.m_Instance, &m_DebugMessengerCreateInfo, nullptr, &m_DebugMessenger);

#endif

	}

	void VulkanInstance::CreateVulkanSurface() const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Create surface and set it global.
		* @note Init with Window's size, but we need resize it to viewport's size after.
		*/
		VK_CHECK(glfwCreateWindowSurface(m_VulkanState.m_Instance, m_VulkanState.m_Windows, nullptr, &m_VulkanState.m_Surface))
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanInstance::InstanceDebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT         messageSeverity, 
		VkDebugUtilsMessageTypeFlagsEXT                messageType, 
		const VkDebugUtilsMessengerCallbackDataEXT*    pCallbackData, 
		void*                                          pUserData
	)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Format string.
		*/
		std::stringstream ss;

		ss <<
		"validation layer:\n			" <<
		"MessageIdNumber: " <<
		pCallbackData->messageIdNumber <<
		"\n			MessageIdName: " <<
		pCallbackData->pMessageIdName;

		if (pCallbackData->cmdBufLabelCount != 0)
		{
			ss << "\n		CmdLabelName: " <<
			pCallbackData->pCmdBufLabels->pLabelName;
		}

		ss <<
		"\n			Message: " <<
		pCallbackData->pMessage;

		switch (messageSeverity)
		{
			/**
			* @brief verbose, unknown level.
			*/
			case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
				SPICES_CORE_TRACE(ss.str());
				break;

			/**
			* @brief info level.
			*/
			case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
				SPICES_CORE_INFO(ss.str());
				break;

			/**
			* @brief warning level.
			*/
			case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
				SPICES_CORE_WARN(ss.str());
				break;

			/**
			* @brief error level.
			*/
			case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
				SPICES_CORE_ERROR(ss.str());
				break;
			
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
				break;
			
			default:
				break;
		}
	
		return VK_FALSE;
	}

	void VulkanInstance::FillDebugMessengerCreateInfo()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Instanced a VkDebugUtilsMessengerCreateInfoEXT with default value.
		*/
		m_DebugMessengerCreateInfo                     = VkDebugUtilsMessengerCreateInfoEXT {};
		m_DebugMessengerCreateInfo.sType               = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		
		m_DebugMessengerCreateInfo.messageSeverity     = 
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT   |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT    ;
		
		m_DebugMessengerCreateInfo.messageType         = 
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT      |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT   | 
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT  ;
		
		m_DebugMessengerCreateInfo.pfnUserCallback     = InstanceDebugCallback;
		m_DebugMessengerCreateInfo.pUserData           = nullptr; // Optional
		m_DebugMessengerCreateInfo.pNext               = nullptr;
	}
}