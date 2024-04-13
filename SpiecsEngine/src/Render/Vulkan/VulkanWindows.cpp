#include "Pchheader.h"
#include "VulkanWindows.h"

#include "Core/Event/KeyEvent.h"
#include "Core/Event/MouseEvent.h"
#include "Core/Event/WindowEvent.h"

namespace Spiecs {

	void OnEvent(Event& e) 
	{
		std::cout << e.GetName() << std::endl;
	}

	VulkanWindows::VulkanWindows(VulkanState& vulkanState, uint32_t width, uint32_t height, const std::string& name)
		: VulkanObject(vulkanState), m_Width(width), m_Height(height), m_WindowsName(name)
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		vulkanState.m_Windows = glfwCreateWindow(m_Width, m_Height, m_WindowsName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(vulkanState.m_Windows, this);

		SetInternalCallBack();
	}

	VulkanWindows::~VulkanWindows()
	{
		glfwDestroyWindow(m_VulkanState.m_Windows);
		glfwTerminate();
	}

	void VulkanWindows::SetInternalCallBack()
	{
		glfwSetErrorCallback([](int error, const char* description) {
			SPIECS_LOG(error);
			SPIECS_LOG(description);
		});

		//glfwSetFramebufferSizeCallback(vulkanState.m_Windows, WindowsResizeCallback);
		glfwSetWindowSizeCallback(m_VulkanState.m_Windows, [](GLFWwindow* window, int width, int height) {
			auto vulkanWindow = reinterpret_cast<VulkanWindows*>(glfwGetWindowUserPointer(window));
			vulkanWindow->m_WindowsResized = true;
			vulkanWindow->m_Width = width;
			vulkanWindow->m_Height = height;

			WindowResizeEvent event(width, height);
			Event::GetEventCallbackFn()(event);
		});

		glfwSetWindowCloseCallback(m_VulkanState.m_Windows, [](GLFWwindow* window) {
			auto vulkanWindow = reinterpret_cast<VulkanWindows*>(glfwGetWindowUserPointer(window));

			WindowCloseEvent event;
			Event::GetEventCallbackFn()(event);
		});

		glfwSetKeyCallback(m_VulkanState.m_Windows, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			auto vulkanWindow = reinterpret_cast<VulkanWindows*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					Event::GetEventCallbackFn()(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					Event::GetEventCallbackFn()(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					Event::GetEventCallbackFn()(event);
					break;
				}
			}
		});

		glfwSetCharCallback(m_VulkanState.m_Windows, [](GLFWwindow* window, unsigned int keycode) {
			auto vulkanWindow = reinterpret_cast<VulkanWindows*>(glfwGetWindowUserPointer(window));

			KeyTypedEvent event(keycode);
			Event::GetEventCallbackFn()(event);
		});

		glfwSetMouseButtonCallback(m_VulkanState.m_Windows, [](GLFWwindow* window, int button, int action, int mods) {
			auto vulkanWindow = reinterpret_cast<VulkanWindows*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					Event::GetEventCallbackFn()(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					Event::GetEventCallbackFn()(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_VulkanState.m_Windows, [](GLFWwindow* window, double xOffest, double yOffest) {
			auto vulkanWindow = reinterpret_cast<VulkanWindows*>(glfwGetWindowUserPointer(window));

			MouseScrolledEvent event((float)xOffest, (float)yOffest);
			Event::GetEventCallbackFn()(event);
		});

		glfwSetCursorPosCallback(m_VulkanState.m_Windows, [](GLFWwindow* window, double xPos, double yPos) {
			auto vulkanWindow = reinterpret_cast<VulkanWindows*>(glfwGetWindowUserPointer(window));

			MouseMovedEvent event((float)xPos, (float)yPos);
			Event::GetEventCallbackFn()(event);
		});

	}
}