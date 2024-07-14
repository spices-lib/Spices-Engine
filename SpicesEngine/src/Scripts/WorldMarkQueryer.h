#pragma once
#include "Core/Core.h"
#include "NativeScript.h"

namespace Spices {

	class WorldMarkQueryer : public NativeScript
	{
	public:
		WorldMarkQueryer();
		virtual ~WorldMarkQueryer() {};

		virtual void OnTick(TimeStep& ts);
		virtual void OnEvent(Event& e) {};
	};
}