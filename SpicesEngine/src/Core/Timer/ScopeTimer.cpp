#include "Pchheader.h"
#include "ScopeTimer.h"

Spices::ScopeTimer::ScopeTimer()
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
}
