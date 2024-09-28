#include "Pchheader.h"
#include "NsightPrefReporterCaptureQueryer.h"
#include "Debugger/Perf/NsightPerfReportGenerator.h"

namespace Spices {

	void NsightPrefReporterCaptureQueryer::OnTick(TimeStep& ts)
	{
		PERFREPORT_CAPTUREFRAME
	}
}