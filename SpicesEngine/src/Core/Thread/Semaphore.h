/**
* @file Semaphore.h
* @brief The Semaphore Class Definitions and Implementation.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"

namespace Spices {

	/**
	* @brief std::semaphore Implementation.
	*/
	class semaphore
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] initSign initial sign.
		*/
		semaphore(int initSign = 0) : m_Sign(initSign) {}

		/**
		* @brief Destructor Function.
		*/
		virtual ~semaphore() = default;

		/**
		* @brief Sign this semaphore with specific value.
		* @param[in] sign .
		*/
		void Sign(int sign)
		{
			std::unique_lock<std::mutex> lock(m_Mutex);

			m_Sign = sign;
			m_Cond.notify_one();
		}

		/**
		* @brief Increase this semaphore sign.
		*/
		void Increase()
		{
			std::unique_lock<std::mutex> lock(m_Mutex);

			++m_Sign;
			m_Cond.notify_one();
		}

		/**
		* @brief Wait this semaphore until it owns equal sign with given.
		* @param[in] sign .
		*/
		void Wait(int sign)
		{
			if (m_Sign.load() != sign)
			{
				std::unique_lock<std::mutex> lock(m_Mutex);

				m_Cond.wait(lock, [&]() { return m_Sign.load() == sign; });
			}
		}

		/**
		* @brief Get this semaphore current sign.
		*/
		const int GetSign() const { return m_Sign.load(); }

	private:

		/**
		* @brief Sign of this semaphore.
		*/
		std::atomic_int m_Sign;

		/**
		* @brief Mutex of this semaphore.
		*/
		std::mutex m_Mutex;

		/**
		* @brief condition variable of this semaphore.
		*/
		std::condition_variable m_Cond;
	};
}