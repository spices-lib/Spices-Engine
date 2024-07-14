/**
* @file Input.h.
* @brief The WindowsInput Class Definitions.
* @author The Cherno.
*/

#pragma once
#include "Core/Core.h"
#include "Input.h"

namespace Spices {

	/**
	* @brief This Class Is Windows Platform Specific Input Class.
	*/
	class WindowsInput : public Input
	{
	protected:

		/**
		* @brief Query If given Key is Pressed.
		* @param[in] keycode In Mouse Button.
		* @return Returns true if given Key is Pressed.
		*/
		virtual bool IsKeyPressedImpl(const int& keycode) override;

		/**
		* @brief Query If given Mouse Button is Pressed.
		* @param[in] button In Mouse Button.
		* @return Returns true if given Key is Pressed.
		*/
		virtual bool IsMouseButtonPressedImpl(const int& button) override;

		/**
		* @brief Query Mouse Position in Windows.
		* @return Returns the pair of Mouse Position in Windows.
		*/
		virtual std::pair<float, float> GetMousePositionImpl() override;

		/**
		* @brief Query Mouse X Position in Windows.
		* @return Returns the Mouse X Position in Windows.
		*/
		virtual float GetMouseXImpl() override;

		/**
		* @brief Query Mouse Y Position in Windows.
		* @return Returns the Mouse Y Position in Windows.
		*/
		virtual float GetMouseYImpl() override;
	};
}