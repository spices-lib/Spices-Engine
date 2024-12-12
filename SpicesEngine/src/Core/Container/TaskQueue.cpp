/**
* @file TaskQueue.h.
* @brief The task_queue Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "TaskQueue.h"

namespace scl {

	void task_queue::PushTask(std::function<void()> fn)
	{
		std::unique_lock<std::mutex> lock(m_Mutex);

		m_Tasks.push(fn);
		++m_NTasks;
	}

	std::function<void()> task_queue::PopTask()
	{
		std::unique_lock<std::mutex> lock(m_Mutex);

		auto ptr = m_Tasks.front();
		m_Tasks.pop();
		--m_NTasks;

		return ptr;
	}

	void task_queue::ExecuteOneTask()
	{
		auto ptr = m_Tasks.front();
		m_Tasks.pop();
		--m_NTasks;

		ptr();
	}
}