/**
* @file VulkanInstance.cpp.
* @brief The VulkanInstance Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "VulkanInstance.h"
#include "Core/Library/StringLibrary.h"

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

		/**
		* @brief Set instance extension.
		*/
		createInfo.enabledExtensionCount = static_cast<uint32_t>(m_ExtensionProperties.size());
		createInfo.ppEnabledExtensionNames = m_ExtensionProperties.data();

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
		createInfo.enabledLayerCount = static_cast<uint32_t>(m_LayerProperties.size());
		createInfo.ppEnabledLayerNames = m_LayerProperties.data();

		/**
		* @brief Set VkDebugUtilsMessengerCreateInfoEXT.
		*/
		FillDebugMessengerCreateInfo();
		createInfo.pNext = &m_DebugMessengerCreateInfo;

		/**
		* @brief Create instance and set it global.
		*/
		VK_CHECK(vkCreateInstance(&createInfo, nullptr, &vulkanState.m_Instance));

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
		/**
		* @brief Destroy the Vulkan Surface Object.
		*/
		vkDestroySurfaceKHR(m_VulkanState.m_Instance, m_VulkanState.m_Surface, nullptr);

		/**
		* @brief Destroy Vulkan's debug message callback function pointer.
		* Working with DEBUG mode.
		*/
#ifdef SPIECS_DEBUG

		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_VulkanState.m_Instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(m_VulkanState.m_Instance, m_DebugMessenger, nullptr);
		}

#endif

		/**
		* @brief Destroy the Vulkan Instance Object.
		*/
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
		* @brief Get glfw extensions requirements.
		*/
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		/**
		* @brief Combine glfw extensions requirements.
		*/
		for (int i = 0; i < glfwExtensionCount; i++) 
		{
			m_ExtensionProperties.push_back(*glfwExtensions);
			glfwExtensions++;
		}

		/**
		* @brief Combine with our extensions requirements.
		* @todo Our instance extensions requirements.
		*/
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
		* @brief Get all instance extensions nums.
		*/
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

		/**
		* @brief Get all instance extensions that supported.
		*/
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		/**
		* @brief Iter all our extensions, check whether all satisfied or not.
		*/
		for (const char* requiredextension : m_ExtensionProperties)
		{
			bool find = false;
			for (auto& target : extensions)
			{
				if (StringLibrary::StringsEqual(requiredextension, target.extensionName))
				{
					find = true;
					break;
				}
			}

			if (!find) return false;
		}

		return true;
	}

	void VulkanInstance::GetLayerRequirements()
	{

#ifdef SPIECS_DEBUG

		/**
		* @brief Combine with our extensions requirements.
		*/
		m_LayerProperties.push_back("VK_LAYER_KHRONOS_validation");

#endif

	}

	bool VulkanInstance::ChecklayerRequirementsSatisfied()
	{
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
		* @brief Iter all our layers, check whether all satisfied or not.
		*/
		for (const char* layerName : m_LayerProperties) 
		{
			bool find = false;
			for (const auto& layerProperties : availableLayers) 
			{
				if (StringLibrary::StringsEqual(layerName, layerProperties.layerName))
				{
					find = true;
					break;
				}
			}

			if (!find) return false;
		}

		return true;
	}

	void VulkanInstance::SetVulkanDebugCallbackFuncPointer()
	{

#ifdef SPIECS_DEBUG

		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_VulkanState.m_Instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) 
		{
			func(m_VulkanState.m_Instance, &m_DebugMessengerCreateInfo, nullptr, &m_DebugMessenger);
		}
		else
		{
			__debugbreak();
		}
#endif
	}

	void VulkanInstance::CreateVulkanSurface()
	{
		/**
		* @brief Create surface and set it global.
		*/
		VK_CHECK(glfwCreateWindowSurface(m_VulkanState.m_Instance, m_VulkanState.m_Windows, nullptr, &m_VulkanState.m_Surface));
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanInstance::InstanceDebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
		VkDebugUtilsMessageTypeFlagsEXT messageType, 
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
		void* pUserData
	)
	{
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}

	void VulkanInstance::FillDebugMessengerCreateInfo()
	{
		/**
		* @brief Instanced a VkDebugUtilsMessengerCreateInfoEXT with default value.
		*/
		m_DebugMessengerCreateInfo = VkDebugUtilsMessengerCreateInfoEXT{};

		/**
		* @brief Fill in sType.
		*/
		m_DebugMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

		/**
		* @brief Fill in messageSeverity.
		*/
		m_DebugMessengerCreateInfo.messageSeverity = 
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT   ;

		/**
		* @brief Fill in messageType.
		*/
		m_DebugMessengerCreateInfo.messageType = 
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT      |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT   | 
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT  ;

		/**
		* @brief Fill in pfnUserCallback.
		*/
		m_DebugMessengerCreateInfo.pfnUserCallback = InstanceDebugCallback;

		/**
		* @brief Fill in pUserData.
		*/
		m_DebugMessengerCreateInfo.pUserData = nullptr; // Optional
	}
}