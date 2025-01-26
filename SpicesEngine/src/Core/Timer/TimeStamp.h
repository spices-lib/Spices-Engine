/**
* @file TimeStamp.h.
* @brief The TimeStamp Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"

namespace Spices {

	/**
	* @brief This class is used for recoring current time.
	*/
	class TimeStamp
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		TimeStamp();

		/**
		* @brief Constructor Function.
		* @param[in] microSecondsSinceEpoch .
		*/
		explicit TimeStamp(int64_t microSecondsSinceEpoch);

		/**
		* @brief Get current time TimeStamp instance.
		* @return Returns current time TimeStamp instance.
		*/
		static TimeStamp Now();

		/**
		* @brief Output time in string format.
		* @return Returns string time.
		*/
		std::string ToString() const;

	private:

		/**
		* @brief Time Counter.
		*/
		int64_t m_MicroSecondsSinceEpoch;
	};
	
}