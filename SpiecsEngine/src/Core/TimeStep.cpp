/**
* @file TimeStep.cpp.
* @brief The TimeStep Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "TimeStep.h"

namespace Spiecs {

	TimeStep::TimeStep()
	{
		/**
		* @brief Init GameTime and FrameTime.
		*/
		m_StartTime = std::chrono::high_resolution_clock::now();
		m_LastTime = std::chrono::high_resolution_clock::now();
	}

	void TimeStep::Flush()
	{
		/**
		* @brief Recording the current time.
		*/
		auto nowTime = std::chrono::high_resolution_clock::now();

		/**
		* @brief Calculate frmae time.
		*/
		m_FrameTime = std::chrono::duration<float, std::chrono::seconds::period>(nowTime - m_LastTime).count();

		/**
		* @brief Calculate game time.
		*/
		m_GameTime = std::chrono::duration<float, std::chrono::seconds::period>(nowTime - m_StartTime).count();

		/**
		* @brief Setting current time.
		*/
		m_LastTime = nowTime;
	}
}