/**
* @file ScopeTimer.h
* @brief The ScopeTimer Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include <chrono>

namespace Spices {

	/**
	* @brief This class helps recording scope time and output logs.
	*/
	class ScopeTimer
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] log .
		*/
		ScopeTimer(const std::string& log);

		/**
		* @brief Destructor Function.
		*/
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

		/**
		* @brief Logs.
		*/
		std::string m_Log;
	};
}