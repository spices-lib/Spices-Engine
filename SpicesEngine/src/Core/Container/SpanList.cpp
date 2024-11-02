/**
* @file SpanList.h.
* @brief The span_list Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "SpanList.h"

namespace scl {

	span::span()
		: m_PageId(0)
		, m_NPages(0)
		, m_Next(nullptr)
		, m_Prev(nullptr)
		, m_FreeList(nullptr)
		, m_UseCount(0)
		, m_IsUse(false)
		, m_BlockSize(0)
	{}

	span_list::span_list()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Init span.
		* @todo new / delete.
		*/
		m_Head = new span;

		m_Head->m_Next = m_Head;
		m_Head->m_Prev = m_Head;
	}

	void span_list::PushFront(span* s)
	{
		SPICES_PROFILE_ZONE;

		Insert(Begin(), s);
	}

	span* span_list::PopFront()
	{
		SPICES_PROFILE_ZONE;

		span* front = m_Head->m_Next;

		Erase(front);

		return front;
	}

	bool span_list::Empty()
	{
		SPICES_PROFILE_ZONE;

		return Begin() == End();
	}

	span* span_list::Begin()
	{
		SPICES_PROFILE_ZONE;

		return m_Head->m_Next;
	}

	span* span_list::End()
	{
		SPICES_PROFILE_ZONE;

		return m_Head;
	}

	void span_list::Insert(span* pos, span* ptr)
	{
		SPICES_PROFILE_ZONE;

		assert(pos);
		assert(ptr);

		span* prev = pos->m_Prev;

		prev->m_Next = ptr;
		ptr->m_Prev = prev;

		ptr->m_Next = pos;
		pos->m_Prev = ptr;
	}

	void span_list::Erase(span* pos)
	{
		SPICES_PROFILE_ZONE;

		assert(pos);
		assert(pos != m_Head);

		span* prev = pos->m_Prev;
		span* next = pos->m_Next;

		prev->m_Next = next;
		next->m_Prev = prev;
	}
}