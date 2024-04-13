#include "Pchheader.h"
#include "Event.h"

namespace Spiecs {

	static Event::EventCallbackFn EventCallback;

	Event::EventCallbackFn Event::GetEventCallbackFn()
	{
		return EventCallback;
	}

	void Event::SetEventCallbackFn(const EventCallbackFn& callback)
	{
		EventCallback = callback;
	}
}