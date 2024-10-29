/**
* @file FreeList.h.
* @brief The free_list Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "FreeList.h"
#include "Core/Memory/MemoryHelper.h"

namespace Spices {

	void free_list::Push(void* obj)
	{
		assert(obj);

		MemoryHelper::ObjNext(obj) = m_Freelist;
		m_Freelist   = obj;

		++m_Size;
	}

	void* free_list::Pop()
	{
		assert(m_Freelist);

		void* obj  = m_Freelist;
		m_Freelist = MemoryHelper::ObjNext(obj);

		--m_Size;

		return obj;
	}

	void free_list::PushRange(void* start, void* end, size_t size)
	{
		MemoryHelper::ObjNext(end) = m_Freelist;
		m_Freelist = start;

		m_Size += size;
	}

	void free_list::PopRange(void*& start, void*& end, size_t n)
	{
		assert(n <= m_Size);

		start = end = m_Freelist;

		for (size_t i = 0; i < n - 1; ++i)
		{
			end = MemoryHelper::ObjNext(end);
		}

		m_Freelist = MemoryHelper::ObjNext(end);
		MemoryHelper::ObjNext(end) = nullptr;
		m_Size -= n;
	}
}