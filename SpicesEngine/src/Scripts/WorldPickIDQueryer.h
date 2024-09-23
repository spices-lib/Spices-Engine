#pragma once
#include "Core/Core.h"
#include "NativeScript.h"
#include "Core/Event/MouseEvent.h"

namespace Spices {

	class ImguiViewport;

	class WorldPickIDQueryer : public NativeScript
	{
	public:
		WorldPickIDQueryer();
		virtual ~WorldPickIDQueryer() {};

		virtual void OnTick(TimeStep& ts) {};
		virtual void OnEvent(Event& e);

	private:
		/**
		* @brief Event OnKeyPressed.
		* We do nothing here.
		* @param[in] e Event Warpper.
		* @return true if we need block the event.
		* @todo Implementate it.
		*/
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

	private:
		uint32_t m_WorldPickID[4] = { 0, 0, 0, 0 };
		std::shared_ptr<ImguiViewport> m_ViewPort;
	};
}