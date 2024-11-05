/**
* @file FreeList.h.
* @brief The free_list Class Implementation.
* @author tcmalloc.
*/

#include "Pchheader.h"
#include "FreeList.h"
#include "Core/Memory/MemoryPool.h"

namespace scl {

	void free_list::Push(void* obj)
	{
		assert(obj);

		/**
		* @brief Insert to begin.
		*/
		Spices::MemoryPool::PointerSpace(obj) = m_Freelist;
		m_Freelist   = obj;

		/**
		* @brief Recoring end pointer.
		*/
		if (m_Size == 0)
		{
			m_EndPointer = obj;
		}

		++m_Size;
	}

	void* free_list::Pop()
	{
		assert(m_Freelist);

		/**
		* @brief Pop from begin.
		*/
		void* obj  = m_Freelist;
		m_Freelist = Spices::MemoryPool::PointerSpace(obj);

		--m_Size;

		/**
		* @brief Unrecoring end pointer.
		*/
		if (m_Size == 0)
		{
			m_EndPointer = nullptr;
		}

		return obj;
	}

	void free_list::PushRange(void* start, void* end, size_t size)
	{
		/**
		* @brief Insert to begin.
		*/
		Spices::MemoryPool::PointerSpace(end) = m_Freelist;
		m_Freelist = start;

		/**
		* @brief Recoring end pointer.
		*/
		if (m_Size == 0)
		{
			m_EndPointer = end;
		}

		m_Size += size;
	}

	void free_list::PopRange(void*& start, void*& end, size_t size)
	{
		assert(size <= m_Size);

		start = end = m_Freelist;

		/**
		* @brief Pop from begin.
		*/
		for (size_t i = 0; i < size - 1; ++i)
		{
			end = Spices::MemoryPool::PointerSpace(end);
		}

		m_Freelist = Spices::MemoryPool::PointerSpace(end);
		Spices::MemoryPool::PointerSpace(end) = nullptr;
		m_Size -= size;

		/**
		* @brief Unrecoring end pointer.
		*/
		if (m_Size == 0)
		{
			m_EndPointer = nullptr;
		}
	}
}