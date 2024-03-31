#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

namespace Spiecs {

	class VulkanInstance : public VulkanObject
	{
	public:
		VulkanInstance(VulkanState& vulkanState, const std::string& name, const std::string& enginename);
		virtual ~VulkanInstance();

	private:
		void FillDebugMessengerCreateInfo();
	private:

		std::vector<const char*> m_ExtensionProperties;
		std::vector<const char*> m_LayerProperties;

		VkDebugUtilsMessengerEXT m_DebugMessenger{};
		VkDebugUtilsMessengerCreateInfoEXT m_DebugMessengerCreateInfo{};
	};
}