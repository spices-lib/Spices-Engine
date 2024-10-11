/**
* @file NsightPerfGPUProfilerContinuous.cpp
* @brief The NsightPerfGPUProfilerContinuous Class Implementation.
* @author Spices
*/

#include "Pchheader.h"
#include "NsightPerfGPUProfilerContinuous.h"
#include "Render/FrameInfo.h"

namespace Spices {

	std::shared_ptr<NsightPerfGPUProfilerContinuous> NsightPerfGPUProfilerContinuous::m_NsightPerfGPUProfilerContinuous;

	NsightPerfGPUProfilerContinuous::NsightPerfGPUProfilerContinuous(VulkanState& state)
		: m_VulkanState(state)
	{
		SPICES_PROFILE_ZONE;

	}
}