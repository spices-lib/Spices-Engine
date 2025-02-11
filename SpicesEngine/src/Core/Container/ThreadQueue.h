/**
* @file ThreadQueue.h.
* @brief The thread_queue Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"

namespace scl {

	/**
	* @brief Thread safe Queue.
	*/
	template<typename T>
	class thread_queue
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		thread_queue() : m_Count(0) {};

		/**
		* @brief Destructor Function.
		*/
		virtual ~thread_queue() = default;

		/**
		* @brief Push a item to this queue.
		* @param[in] item Queue item.
		*/
		void Push(T&& item);

		/**
		* @brief Pop a item from this queue.
		* @return Returns item.
		*/
		T Pop();

		/**
		* @brief Is this queue is empty.
		* @reutrn Returns true if empty.
		*/
		bool IsEmpty() const { return m_Count.load() == 0; }

		/**
		* @brief Clear this queue.
		*/
		void Clear();

	private:

		/**
		* @brief Mutex of this queue.
		*/
		std::mutex m_Mutex;

		/**
		* @brief Not empty condition.
		*/
		std::condition_variable m_NotEmpty;

		/**
		* @brief Count of tasks.
		*/
		std::atomic_int m_Count;

		/**
		* @brief This wrapped queue.
		*/
		std::queue<T> m_Queue;
	};

	template<typename T>
	void thread_queue<T>::Push(T&& item)
	{
		std::unique_lock<std::mutex> lock(m_Mutex);

		m_Queue.push(item);
		++m_Count;

		m_NotEmpty.notify_all();
	}

	template<typename T>
	inline T thread_queue<T>::Pop()
	{
		std::unique_lock<std::mutex> lock(m_Mutex);

		if (IsEmpty())
			m_NotEmpty.wait(lock, [&](){ return !IsEmpty(); });

		auto ptr = m_Queue.front();
		m_Queue.pop();
		--m_Count;

		return std::move(ptr);
	}

	template<typename T>
	inline void thread_queue<T>::Clear()
	{
		std::unique_lock<std::mutex> lock(m_Mutex);

		std::queue<T>().swap(m_Queue);
		m_Count = 0;
	}
}