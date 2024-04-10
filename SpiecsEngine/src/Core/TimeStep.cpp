#include "Pchheader.h"
#include "TimeStep.h"

namespace Spiecs {

	TimeStep::TimeStep()
	{
		m_StartTime = std::chrono::high_resolution_clock::now();
		m_LastTime = std::chrono::high_resolution_clock::now();
	}

	void TimeStep::Flush()
	{
		auto nowTime = std::chrono::high_resolution_clock::now();
		m_FrameTime = std::chrono::duration<float, std::chrono::seconds::period>(nowTime - m_LastTime).count();
		m_GameTime = std::chrono::duration<float, std::chrono::seconds::period>(nowTime - m_StartTime).count();
		m_LastTime = nowTime;
	}
}