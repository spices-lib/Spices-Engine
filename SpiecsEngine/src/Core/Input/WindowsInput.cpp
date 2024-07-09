/**
* @file Input.h.
* @brief The WindowsInput Class Implementation.
* @author The Cherno.
*/

#include "Pchheader.h"
#include "WindowsInput.h"
#include "Render/Vulkan/VulkanRenderBackend.h"

#include <GLFW/glfw3.h>

namespace Spiecs {
	
	/**
	* @brief Instance a WindowsInput single instance.
	*/
	std::unique_ptr<Input> Input::s_Instance = std::make_unique<WindowsInput>();

	bool WindowsInput::IsKeyPressedImpl(const int& keycode)
	{
		/**
		* @brief Get GLFW Window Pointer.
		*/
		const auto window = VulkanRenderBackend::GetState().m_Windows;

		/**
		* @brief Query GLFW Window Key State.
		*/
		const auto state = glfwGetKey(window, keycode);
		
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(const int& button)
	{
		/**
		* @brief Get GLFW Window Pointer.
		*/
		const auto window = VulkanRenderBackend::GetState().m_Windows;

		/**
		* @brief Query GLFW Window Mouse State.
		*/
		const auto state = glfwGetMouseButton(window, button);
		
		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		/**
		* @brief Get GLFW Window Pointer.
		*/
		const auto window = VulkanRenderBackend::GetState().m_Windows;

		/**
		* @brief Query GLFW Window Cursor Position.
		*/
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		
		return { static_cast<float>(xPos), static_cast<float>(yPos) };
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return y;
	}
}