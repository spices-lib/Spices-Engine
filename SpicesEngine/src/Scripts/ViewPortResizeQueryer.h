/**
* @file ViewPortResizeQueryer.h.
* @brief The ViewPortResizeQueryer Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "NativeScript.h"

namespace Spices {

	/**
    * @brief Forward declare. 
    */
	class ImguiViewport;

	/**
	* @brief Script of handle viewport resize event. 
	*/
	class ViewPortResizeQueryer : public NativeScript
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		ViewPortResizeQueryer() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~ViewPortResizeQueryer() override = default;

		/**
		* @brief This interface defines the behave on specific component tick every frame.
		* @param[in] ts TimeStep.
		*/
		virtual void OnTick(TimeStep& ts);

		/**
		* @brief This interface defines the behave on specific component event happened.
		* @param[in] e Event.
		*/
		virtual void OnEvent(Event& e) {};

	private:
		
		/**
		* @brief viewport pointer.
		*/
		std::shared_ptr<ImguiViewport> m_ViewPort;
	};

}