/**
* @file TimeStep.h.
* @brief Time Counting for our Engine. 
* The TimeStep Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "chrono"

namespace Spiecs {

	/**
	* @brief This Class handles our engine timestep during frames.
	* Golbal Unique.
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
		virtual ~TimeStep() {};

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaver.
		*/
		TimeStep(const TimeStep&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaver.
		*/
		TimeStep& operator=(const TimeStep&) = delete;

		/**
		* @brief Get timestep during frames.
		* @return timestep(s) during frames.
		*/
		float ft() { return m_FrameTime; };

		/**
		* @brief Get timestep since Engine Start.
		* @return timestep(s) since Engine Start.
		*/
		float gt() { return m_GameTime; };

		/**
		* @brief Refrush time in each engine loop.
		* @return no return.
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
		* @brief timestep(s) during frames.
		*/
		float m_FrameTime = 0;

		/**
		* @brief timestep(s) since Engine Start.
		*/
		float m_GameTime = 0;
	};
}