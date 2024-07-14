/**
* @file Input.h.
* @brief The Input Class Definitions.
* @author The Cherno.
*/

#pragma once
#include "Core/Core.h"

namespace Spices {
	
	/**
	* @brief This Class Is a wrapper of Platform Specific Input Query.
	*/
	class Input
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		Input() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~Input() = default;
		
		/**
		* @brief Query If given Key is Pressed.
		* @param[in] keycode In Key Code.
		* @return Returns true if given Key is Pressed.
		*/
		static bool IsKeyPressed(const int& keycode) { return s_Instance->IsKeyPressedImpl(keycode); }

		/**
		* @brief Query If given Mouse Button is Pressed.
		* @param[in] button In Mouse Button.
		* @return Returns true if given Key is Pressed.
		*/
		static bool IsMouseButtonPressed(const int& button) { return s_Instance->IsMouseButtonPressedImpl(button); }

		/**
		* @brief Query Mouse Position in Windows.
		* @return Returns the pair of Mouse Position in Windows.
		*/
		static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }

		/**
		* @brief Query Mouse X Position in Windows.
		* @return Returns the Mouse X Position in Windows.
		*/
		static float GetMouseX() { return s_Instance->GetMouseXImpl(); }

		/**
		* @brief Query Mouse Y Position in Windows.
		* @return Returns the Mouse Y Position in Windows.
		*/
		static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
		
	protected:

		/**
		* @brief Query If given Key is Pressed.
		* Interface for platform Implementation. 
		* @param[in] keycode In Mouse Button.
		* @return Returns true if given Key is Pressed.
		*/
		virtual bool IsKeyPressedImpl(const int& keycode) = 0;

		/**
		* @brief Query If given Mouse Button is Pressed.
		* Interface for platform Implementation. 
		* @param[in] button In Mouse Button.
		* @return Returns true if given Key is Pressed.
		*/
		virtual bool IsMouseButtonPressedImpl(const int& button) = 0;

		/**
		* @brief Query Mouse Position in Windows.
		* Interface for platform Implementation.
		* @return Returns the pair of Mouse Position in Windows.
		*/
		virtual std::pair<float, float> GetMousePositionImpl() = 0;

		/**
		* @brief Query Mouse X Position in Windows.
		* Interface for platform Implementation.
		* @return Returns the Mouse X Position in Windows.
		*/
		virtual float GetMouseXImpl() = 0;

		/**
		* @brief Query Mouse Y Position in Windows.
		* Interface for platform Implementation.
		* @return Returns the Mouse Y Position in Windows.
		*/
		virtual float GetMouseYImpl() = 0;
		
	private:

		/**
		* @brief Platform Specific Input Class.
		*/
		static std::unique_ptr<Input> s_Instance;
	};

}