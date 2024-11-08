/**
* @file SpanList.h.
* @brief The span_list Class Implementation.
* @author tcmalloc.
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
		m_Head = new span;

		m_Head->m_Next = m_Head;
		m_Head->m_Prev = m_Head;
	}

	span_list::~span_list()
	{
		if (m_Head)
		{
			delete m_Head;
			m_Head = nullptr;
		}
	}

	void span_list::PushFront(span* s)
	{
		Insert(Begin(), s);
	}

	span* span_list::PopFront()
	{
		span* front = m_Head->m_Next;

		Erase(front);

		return front;
	}

	bool span_list::Empty()
	{
		return Begin() == End();
	}

	span* span_list::Begin() const
	{
		return m_Head->m_Next;
	}

	span* span_list::End() const
	{
		return m_Head;
	}

	void span_list::Insert(span* pos, span* ptr)
	{
		assert(pos);
		assert(ptr);

		span* prev = pos->m_Prev;

		prev->m_Next = ptr;
		ptr->m_Prev = prev;

		ptr->m_Next = pos;
		pos->m_Prev = ptr;
	}

	void span_list::Erase(span* pos) const
	{
		assert(pos);
		assert(pos != m_Head);

		span* prev = pos->m_Prev;
		span* next = pos->m_Next;

		prev->m_Next = next;
		next->m_Prev = prev;

		pos->m_Next = nullptr;
		pos->m_Prev = nullptr;
	}
}