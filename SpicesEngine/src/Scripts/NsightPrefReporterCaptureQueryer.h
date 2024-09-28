#pragma once
#include "Core/Core.h"
#include "NativeScript.h"

namespace Spices {

	class NsightPrefReporterCaptureQueryer : public NativeScript
	{
	public:
		NsightPrefReporterCaptureQueryer() {};
		virtual ~NsightPrefReporterCaptureQueryer() {};

		virtual void OnTick(TimeStep& ts);
		virtual void OnEvent(Event& e) {};
	};

}