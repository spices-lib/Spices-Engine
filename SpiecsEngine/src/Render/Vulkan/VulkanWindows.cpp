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
		SPIECS_PROFILE_ZONE;

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
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Destroy glfw window.
		*/
		glfwDestroyWindow(m_VulkanState.m_Windows);

		/**
		* @brief Destroy glfw relative object.
		*/
		glfwTerminate();
	}

	void VulkanWindows::SetInternalCallBack() const
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Error event Callback.
		* @todo Butter Log.
		*/
		glfwSetErrorCallback([](int error, const char* description) 
		{
			//SPIECS_CORE_INFO(error);
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
			const auto vulkanWindow = static_cast<VulkanWindows*>(glfwGetWindowUserPointer(window));

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
			* @brief Create an specific event.
			*/
			KeyTypedEvent event(static_cast<int>(keycode));

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
		glfwSetScrollCallback(m_VulkanState.m_Windows, [](GLFWwindow* window, double xOffset, double yOffset) 
		{
			/**
			* @brief Create an specific event.
			*/
			MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));

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
			* @brief Create an specific event.
			*/
			MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));

			/**
			* @brief Execute the global event function pointer by passing the specific event.
			*/
			Event::GetEventCallbackFn()(event);
		});
	}
}