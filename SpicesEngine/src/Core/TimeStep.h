/**
* @file TimeStep.h.
* @brief Time Counting for our Engine. 
* The TimeStep Class Definitions.
* @author Spices.
*/

#pragma once
#include "chrono"

namespace Spices {

	/**
	* @brief This Class handles our engine time step during frames.
	* Global Unique.
	* @todo Entity timeStep System.
	* @todo operator() return m_FrameTime.
	* @todo ms / s / us counting.
	*/
	class TimeStep
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		TimeStep();

		/**
		* @brief Destructor Function.
		*/
		virtual ~TimeStep() = default;

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaves.
		*/
		TimeStep(const TimeStep&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaves.
		*/
		TimeStep& operator=(const TimeStep&) = delete;

		/**
		* @brief Get time step during frames.
		* @return time step(s) during frames.
		*/
		float ft() const { return m_FrameTime; }

		/**
		* @brief Get time step since Engine Start.
		* @return time step(s) since Engine Start.
		*/
		float gt() const { return m_GameTime; }

		/**
		* @brief Refresh time in each engine loop.
		*/
		void Flush();

	public:

		/**
		* @brief Engine Start time.
		*/
		std::chrono::steady_clock::time_point m_StartTime;

		/**
		* @brief Last frame time.
		*/
		std::chrono::steady_clock::time_point m_LastTime;

		/**
		* @brief time step(s) during frames.
		*/
		float m_FrameTime = 0;

		/**
		* @brief time step(s) since Engine Start.
		*/
		float m_GameTime = 0;
	};
}