/**
* @file ThreadPool.h
* @brief The ThreadPool Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ThreadPool.h"

namespace Spices {

	uint32_t Thread::m_GeneraterId = 0;

	Thread::Thread(ThreadFunc func)
		: m_Func(func)
		, m_ThreadId(m_GeneraterId++)
	{}

	void Thread::Start()
	{
		SPICES_PROFILE_ZONE;

		std::thread t(m_Func, m_ThreadId);
		t.detach();
	}
}