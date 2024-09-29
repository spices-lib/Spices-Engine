/**
* @file Log.h
* @brief The Log Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include <chrono>

namespace Spices {

	class ScopeTimer
	{
	public:

		ScopeTimer(const std::string& log);

		virtual ~ScopeTimer();


	private:

		/**
		* @brief In Scope time.
		*/
		std::chrono::steady_clock::time_point m_InTime;

		/**
		* @brief Leave Scope time.
		*/
		std::chrono::steady_clock::time_point m_LeaveTime;
	};
}