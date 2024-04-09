#include "pchheader.h"
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
		float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(nowTime - m_LastTime).count();
		float gameTime = std::chrono::duration<float, std::chrono::seconds::period>(nowTime - m_StartTime).count();
		m_LastTime = nowTime;
	}
}