#pragma once
#include "Core/Core.h"

namespace Spiecs {

	class VulkanInstance
	{
	public:
		VulkanInstance(const std::string& name, const std::string& enginename, GLFWwindow* window);
		virtual ~VulkanInstance();

		VulkanInstance(const VulkanInstance&) = delete;
		VulkanInstance& operator=(const VulkanInstance&) = delete;

		VkInstance& Get() { return m_Instance; };
		VkSurfaceKHR& GetSurface() { return m_Surface; };

	private:
		void FillDebugMessengerCreateInfo();
	private:
		VkInstance m_Instance;
		VkSurfaceKHR m_Surface;

		std::vector<const char*> m_ExtensionProperties;
		std::vector<const char*> m_LayerProperties;

		VkDebugUtilsMessengerEXT m_DebugMessenger;
		VkDebugUtilsMessengerCreateInfoEXT m_DebugMessengerCreateInfo;
	};
}