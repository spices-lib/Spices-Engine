#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

namespace Spiecs {

	class VulkanWindows : public VulkanObject
	{
	public:
		VulkanWindows(VulkanState& vulkanState, uint32_t width, uint32_t height, const std::string& name);
		virtual ~VulkanWindows();

		bool IsResized() { return m_WindowsResized; };
		void SetResized(bool b) { m_WindowsResized = b; };

	private:
		static void WindowsResizeCallback(GLFWwindow* window, int width, int height);
	private:

		uint32_t m_Width;
		uint32_t m_Height;
		std::string m_WindowsName;
		bool m_WindowsResized = false;
	};
}