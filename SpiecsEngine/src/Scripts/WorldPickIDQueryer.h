#pragma once
#include "Core/Core.h"
#include "NativeScript.h"
#include "Core/Event/MouseEvent.h"

namespace Spiecs {

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
		float m_WorldPickID[4];
		float* m_WorldPickIDMemory;
		std::shared_ptr<ImguiViewport> m_ViewPort;
	};
}