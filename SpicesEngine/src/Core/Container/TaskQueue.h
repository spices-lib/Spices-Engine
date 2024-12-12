/**
* @file TaskQueue.h.
* @brief The task_queue Class Definitions.
* @author tcmalloc.
*/

#pragma once
#include "Core/Core.h"

namespace scl {

	/**
	* @brief Queue of store function pointer.
	*/
	class task_queue
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		task_queue() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~task_queue() = default;

		/**
		* @brief Push a task to this queue.
		* @param[in] fn function pointer.
		*/
		void PushTask(std::function<void()> fn);

		/**
		* @brief Pop a task from this queue.
		* @return Returns task.
		*/
		std::function<void()> PopTask();

		/**
		* @brief Execute a task in this queue.
		*/
		void ExecuteOneTask();

		/**
		* @brief Is this queue is empty.
		* @reutrn Returns true if empty.
		*/
		bool IsEmpty() { return m_NTasks.load() == 0; };

	private:

		/**
		* @brief Mutex of this queue.
		*/
		std::mutex m_Mutex;

		/**
		* @brief Count of tasks.
		*/
		std::atomic_int m_NTasks;

		/**
		* @brief Tasks of this queue.
		*/
		std::queue<std::function<void()>> m_Tasks;
	};
}