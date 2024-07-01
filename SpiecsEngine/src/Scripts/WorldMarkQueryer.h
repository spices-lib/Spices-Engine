#pragma once
#include "Core/Core.h"
#include "NativeScript.h"

namespace Spiecs {

	class WorldMarkQueryer : public NativeScript
	{
	public:
		WorldMarkQueryer();
		virtual ~WorldMarkQueryer() {};

		virtual void OnTick(TimeStep& ts);
		virtual void OnEvent(Event& e) {};
	};
}