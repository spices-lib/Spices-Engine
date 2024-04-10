#include "Pchheader.h"
#include "VulkanWindows.h"

namespace Spiecs {

	VulkanWindows::VulkanWindows(VulkanState& vulkanState, uint32_t width, uint32_t height, const std::string& name)
		: VulkanObject(vulkanState), m_Width(width), m_Height(height), m_WindowsName(name)
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		vulkanState.m_Windows = glfwCreateWindow(m_Width, m_Height, m_WindowsName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(vulkanState.m_Windows, this);
		glfwSetFramebufferSizeCallback(vulkanState.m_Windows, WindowsResizeCallback);
	}

	VulkanWindows::~VulkanWindows()
	{
		glfwDestroyWindow(m_VulkanState.m_Windows);
		glfwTerminate();
	}

	void VulkanWindows::WindowsResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto lveWindow = reinterpret_cast<VulkanWindows*>(glfwGetWindowUserPointer(window));
		lveWindow->m_WindowsResized = true;
		lveWindow->m_Width = width;
		lveWindow->m_Height = height;
	}
}