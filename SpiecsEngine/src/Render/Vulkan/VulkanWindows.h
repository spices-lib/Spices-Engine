#pragma once
#include "Core/Core.h"

namespace Spiecs {

	class VulkanWindows
	{
	public:
		VulkanWindows(uint32_t width, uint32_t height, const std::string& name);
		virtual ~VulkanWindows();

		VulkanWindows(const VulkanWindows&) = delete;
		VulkanWindows& operator=(const VulkanWindows&) = delete;

		GLFWwindow* Get() { return m_Windows; };

	private:
		static void WindowsResizeCallback(GLFWwindow* window, int width, int height);
	private:
		GLFWwindow* m_Windows;

		uint32_t m_Width;
		uint32_t m_Height;
		std::string m_WindowsName;
		bool m_WindowsResized = false;
	};
}