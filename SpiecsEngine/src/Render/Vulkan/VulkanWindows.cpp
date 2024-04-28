/**
* @file VulkanWindows.cpp.
* @brief The VulkanWindows Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "VulkanWindows.h"

#include "Core/Event/KeyEvent.h"
#include "Core/Event/MouseEvent.h"
#include "Core/Event/WindowEvent.h"

namespace Spiecs {

	VulkanWindows::VulkanWindows(VulkanState& vulkanState, const WindowInfo& initInfo)
		: VulkanObject(vulkanState), m_WindowInfo(initInfo)
	{
		/**
		* @brief glfw initialize.
		*/
		glfwInit();

		/**
		* @brief Set glfw not use OpenGl api.
		*/
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		/**
		* @brief Set glfw enable resize feature. 
		*/
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		/**
		* @brief Create glfwWindow.
		*/
		vulkanState.m_Windows = glfwCreateWindow(initInfo.width, initInfo.height, initInfo.name.c_str(), nullptr, nullptr);

		/**
		* @brief Set glfw call back object pointer.
		*/
		glfwSetWindowUserPointer(vulkanState.m_Windows, this);

		/**
		* @brief Set all needed GLFW events call back.
		*/
		SetInternalCallBack();
	}

	VulkanWindows::~VulkanWindows()
	{
		/**
		* @brief Destroy glfw window.
		*/
		glfwDestroyWindow(m_VulkanState.m_Windows);

		/**
		* @brief Destroy glfw relative object.
		*/
		glfwTerminate();
	}

	void VulkanWindows::SetInternalCallBack()
	{
		/**
		* @brief Error event Callback.
		* @todo Butter Log.
		*/
		glfwSetErrorCallback([](int error, const char* description) 
		{
			SPIECS_CORE_INFO(error);
			SPIECS_CORE_INFO(description);
		});

		/**
		* @brief Framebuffer resize event Callback.
		* @note Not in use now.
		*/
		//glfwSetFramebufferSizeCallback(vulkanState.m_Windows, WindowsResizeCallback);

		/**
		* @brief Window resize event Callback.
		*/
		glfwSetWindowSizeCallback(m_VulkanState.m_Windows, [](GLFWwindow* window, int width, int height) 
		{
			/**
			* @brief Reinterpretate the pointer to this class.
			*/
			auto vulkanWindow = reinterpret_cast<VulkanWindows*>(glfwGetWindowUserPointer(window));

			/**
			* @brief Set this class's variable.
			*/
			vulkanWindow->m_WindowsResized = true;
			vulkanWindow->m_WindowInfo.width = width;
			vulkanWindow->m_WindowInfo.height = height;

			/**
			* @brief Create an specific event.
			*/
			WindowResizeEvent event(width, height);

			/**
			* @brief Execute the global event function pointer by passing the specific event.
			*/
			Event::GetEventCallbackFn()(event);
		});

		/**
		* @brief Window close event Callback.
		*/
		glfwSetWindowCloseCallback(m_VulkanState.m_Windows, [](GLFWwindow* window) 
		{
			/**
			* @brief Reinterpretate the pointer to this class.
			*/
			auto vulkanWindow = reinterpret_cast<VulkanWindows*>(glfwGetWindowUserPointer(window));

			/**
			* @brief Create an specific event.
			*/
			WindowCloseEvent event;

			/**
			* @brief Execute the global event function pointer by passing the specific event.
			*/
			Event::GetEventCallbackFn()(event);
		});

		/**
		* @brief Key event Callback.
		*/
		glfwSetKeyCallback(m_VulkanState.m_Windows, [](GLFWwindow* window, int key, int scancode, int action, int mods) 
		{
			/**
			* @brief Reinterpretate the pointer to this class.
			*/
			auto vulkanWindow = reinterpret_cast<VulkanWindows*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
				case GLFW_PRESS:
				{
					/**
					* @brief Create an specific event.
					*/
					KeyPressedEvent event(key, 0);

					/**
					* @brief Execute the global event function pointer by passing the specific event.
					*/
					Event::GetEventCallbackFn()(event);

					break;
				}
				case GLFW_RELEASE:
				{
					/**
					* @brief Create an specific event.
					*/
					KeyReleasedEvent event(key);

					/**
					* @brief Execute the global event function pointer by passing the specific event.
					*/
					Event::GetEventCallbackFn()(event);

					break;
				}
				case GLFW_REPEAT:
				{
					/**
					* @brief Create an specific event.
					*/
					KeyPressedEvent event(key, 1);

					/**
					* @brief Execute the global event function pointer by passing the specific event.
					*/
					Event::GetEventCallbackFn()(event);

					break;
				}
			}
		});

		/**
		* @brief Key Input event Callback.
		*/
		glfwSetCharCallback(m_VulkanState.m_Windows, [](GLFWwindow* window, unsigned int keycode) 
		{
			/**
			* @brief Reinterpretate the pointer to this class.
			*/
			auto vulkanWindow = reinterpret_cast<VulkanWindows*>(glfwGetWindowUserPointer(window));

			/**
			* @brief Create an specific event.
			*/
			KeyTypedEvent event(keycode);

			/**
			* @brief Execute the global event function pointer by passing the specific event.
			*/
			Event::GetEventCallbackFn()(event);
		});

		/**
		* @brief Mouse Button event Callback.
		*/
		glfwSetMouseButtonCallback(m_VulkanState.m_Windows, [](GLFWwindow* window, int button, int action, int mods) 
		{
			/**
			* @brief Reinterpretate the pointer to this class.
			*/
			auto vulkanWindow = reinterpret_cast<VulkanWindows*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
				case GLFW_PRESS:
				{
					/**
					* @brief Create an specific event.
					*/
					MouseButtonPressedEvent event(button);

					/**
					* @brief Execute the global event function pointer by passing the specific event.
					*/
					Event::GetEventCallbackFn()(event);

					break;
				}
				case GLFW_RELEASE:
				{
					/**
					* @brief Create an specific event.
					*/
					MouseButtonReleasedEvent event(button);

					/**
					* @brief Execute the global event function pointer by passing the specific event.
					*/
					Event::GetEventCallbackFn()(event);

					break;
				}
			}
		});

		/**
		* @brief Mouse Scroll event Callback.
		*/
		glfwSetScrollCallback(m_VulkanState.m_Windows, [](GLFWwindow* window, double xOffest, double yOffest) 
		{
			/**
			* @brief Reinterpretate the pointer to this class.
			*/
			auto vulkanWindow = reinterpret_cast<VulkanWindows*>(glfwGetWindowUserPointer(window));

			/**
			* @brief Create an specific event.
			*/
			MouseScrolledEvent event((float)xOffest, (float)yOffest);

			/**
			* @brief Execute the global event function pointer by passing the specific event.
			*/
			Event::GetEventCallbackFn()(event);
		});

		/**
		* @brief Mouse Move event Callback.
		*/
		glfwSetCursorPosCallback(m_VulkanState.m_Windows, [](GLFWwindow* window, double xPos, double yPos) 
		{
			/**
			* @brief Reinterpretate the pointer to this class.
			*/
			auto vulkanWindow = reinterpret_cast<VulkanWindows*>(glfwGetWindowUserPointer(window));

			/**
			* @brief Create an specific event.
			*/
			MouseMovedEvent event((float)xPos, (float)yPos);

			/**
			* @brief Execute the global event function pointer by passing the specific event.
			*/
			Event::GetEventCallbackFn()(event);
		});
	}
}