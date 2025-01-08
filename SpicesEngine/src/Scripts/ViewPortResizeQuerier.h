/**
* @file ViewPortResizeQuerier.h.
* @brief The ViewPortResizeQuerier Definitions.
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
	class ViewPortResizeQuerier : public NativeScript
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		ViewPortResizeQuerier() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~ViewPortResizeQuerier() override = default;

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
		std::weak_ptr<ImguiViewport> m_ViewPort;
	};

}