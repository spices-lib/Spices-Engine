/**
* @file WorldPickIDQuerier.h.
* @brief The WorldPickIDQuerier Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "NativeScript.h"
#include "Core/Event/MouseEvent.h"

namespace Spices {
	
	/**
	* @brief Forward declare. 
	*/
	class ImguiViewport;

	/**
	* @brief Script of handle world entity pick.
	*/
	class WorldPickIDQuerier : public NativeScript
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		WorldPickIDQuerier() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~WorldPickIDQuerier() override = default;

		/**
		* @brief This interface defines the behave on specific component tick every frame.
		* @param[in] ts TimeStep.
		*/
		virtual void OnTick(TimeStep& ts) override {};

		/**
		* @brief This interface defines the behave on specific component event happened.
		* @param[in] e Event.
		*/
		virtual void OnEvent(Event& e) override;

	private:
		
		/**
		* @brief Event OnKeyPressed.
		* @param[in] e Event Wrapper.
		* @return true if we need block the event.
		*/
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

	private:
		
		/**
		* @brief World Picked entity id (only use channel 0).
		*/
		float m_WorldPickID[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

		/**
		* @brief viewport pointer.
		*/
		std::weak_ptr<ImguiViewport> m_ViewPort;
	};
}