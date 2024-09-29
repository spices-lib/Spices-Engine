/**
* @file ScopeTimer.cpp.
* @brief The ScopeTimer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ScopeTimer.h"

namespace Spices {

	Spices::ScopeTimer::ScopeTimer(const std::string& log)
		: m_Log(log)
	{
		/**
		* @brief Init In Time.
		*/
		m_InTime = std::chrono::high_resolution_clock::now();
	}

	Spices::ScopeTimer::~ScopeTimer()
	{
		/**
		* @brief Init Leave Time.
		*/
		m_LeaveTime = std::chrono::high_resolution_clock::now();

		std::stringstream ss;
		ss << "ScopeTimer : " << std::chrono::duration<float, std::chrono::milliseconds::period>(m_LeaveTime - m_InTime).count() << " milliseconds \n" << m_Log;

		std::cout << ss.str() << std::endl;
	}

}