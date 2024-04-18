/**
* @file VulkanInstance.cpp.
* @brief The VulkanInstance Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "VulkanInstance.h"

namespace Spiecs {

	VulkanInstance::VulkanInstance(VulkanState& vulkanState, const std::string& name, const std::string& enginename)
		: VulkanObject(vulkanState)
	{
		/**
		* @brief Create VkApplicationInfo struct.
		*/
		VkApplicationInfo appInfo = CreateApplicationInfo(name, enginename);

		/**
		* @brief Create VkInstanceCreateInfo struct.
		*/
		VkInstanceCreateInfo createInfo = CreateInstanceCreateInfo(appInfo);

		/**
		* @brief Get all instance extension requirements our engine needede.
		*/
		GetExtensionRequirements();

		/**
		* @brief Iter all our extensions, check whether all satisfied or not.
		*/
		if (!CheckExtensionRequirementsSatisfied()) return;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(m_ExtensionProperties.size());
		createInfo.ppEnabledExtensionNames = m_ExtensionProperties.data();

		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;

#ifdef SPIECS_DEBUG

		m_LayerProperties = { "VK_LAYER_KHRONOS_validation" };

		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : m_LayerProperties) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) return;
		}

		createInfo.enabledLayerCount = static_cast<uint32_t>(m_LayerProperties.size());
		createInfo.ppEnabledLayerNames = m_LayerProperties.data();

		FillDebugMessengerCreateInfo();
		createInfo.pNext = &m_DebugMessengerCreateInfo;
#endif
		VK_CHECK(vkCreateInstance(&createInfo, nullptr, &vulkanState.m_Instance));

#ifdef SPIECS_DEBUG

		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vulkanState.m_Instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(vulkanState.m_Instance, &m_DebugMessengerCreateInfo, nullptr, &m_DebugMessenger);
		}
		else
		{
			__debugbreak();
		}
#endif

		SPIECS_LOG("VkInstance Create Succeed!!!");


		VK_CHECK(glfwCreateWindowSurface(vulkanState.m_Instance, m_VulkanState.m_Windows, nullptr, &vulkanState.m_Surface));
		SPIECS_LOG("VkSurfaceKHR Create Succeed!!!");

	}

	VulkanInstance::~VulkanInstance()
	{
		vkDestroySurfaceKHR(m_VulkanState.m_Instance, m_VulkanState.m_Surface, nullptr);

#ifdef SPIECS_DEBUG
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_VulkanState.m_Instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(m_VulkanState.m_Instance, m_DebugMessenger, nullptr);
		}
#endif
		vkDestroyInstance(m_VulkanState.m_Instance, nullptr);
	}

	VkApplicationInfo VulkanInstance::CreateApplicationInfo(const std::string& name, const std::string& enginename)
	{
		/**
		* @brief Instanced a VkApplicationInfo with default value.
		*/
		VkApplicationInfo appInfo = {};

		/**
		* @brief Fill in sType.
		*/
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

		/**
		* @brief Fill in pApplicationName.
		*/
		appInfo.pApplicationName = name.c_str();

		/**
		* @brief Fill in applicationVersion.
		*/
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);

		/**
		* @brief Fill in pEngineName.
		*/
		appInfo.pEngineName = enginename.c_str();

		/**
		* @brief Fill in engineVersion.
		*/
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

		/**
		* @brief Fill in apiVersion.
		*/
		appInfo.apiVersion = VK_API_VERSION_1_0;

		return appInfo;
	}

	VkInstanceCreateInfo VulkanInstance::CreateInstanceCreateInfo(const VkApplicationInfo& appInfo)
	{
		/**
		* @brief Instanced a VkInstanceCreateInfo with default value.
		*/
		VkInstanceCreateInfo createInfo = {};

		/**
		* @brief Fill in sType.
		*/
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

		/**
		* @brief Fill in pApplicationInfo.
		*/
		createInfo.pApplicationInfo = &appInfo;

		return createInfo;
	}

	void VulkanInstance::GetExtensionRequirements()
	{
		/**
		* @brief Get glfw requirements.
		*/
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		/**
		* @brief Combine with our extensions requirements.
		*/
		for (int i = 0; i < glfwExtensionCount; i++) 
		{
			m_ExtensionProperties.push_back(*glfwExtensions);
			glfwExtensions++;
		}

#ifdef SPIECS_DEBUG

		/**
		* @brief To enable validation layer, we need this instance extension.
		*/
		m_ExtensionProperties.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

#endif
	}

	bool VulkanInstance::CheckExtensionRequirementsSatisfied()
	{
		/**
		* @brief Get all instance extensions that supported.
		*/
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		/**
		* @brief Iter all our extensions, check whether all satisfied or not.
		*/
		bool fullmatch = true;
		for (int i = 0; i < m_ExtensionProperties.size(); i++) {
			const char* requiredextension = m_ExtensionProperties[i];
			bool match = false;
			for (int j = 0; j < extensionCount; j++)
			{
				VkExtensionProperties& target = extensions[j];
				match = strcmp(requiredextension, target.extensionName) == 0;

				if (match) break;
			}
			if (!match) fullmatch = false; break;
		}

		if (!fullmatch) return false;

		return true;
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanInstance::InstanceDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}

	void VulkanInstance::FillDebugMessengerCreateInfo()
	{
		m_DebugMessengerCreateInfo = VkDebugUtilsMessengerCreateInfoEXT{};
		m_DebugMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		m_DebugMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		m_DebugMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		m_DebugMessengerCreateInfo.pfnUserCallback = InstanceDebugCallback;
		m_DebugMessengerCreateInfo.pUserData = nullptr; // Optional
	}
}