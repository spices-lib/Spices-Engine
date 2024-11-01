#include "Pchheader.h"
#include "SpanList.h"

namespace scl {

	span_list::span_list()
	{
		m_Head = new span;

		m_Head->m_Next = m_Head;
		m_Head->m_Prev = m_Head;
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
		return m_Head == m_Head->m_Next;
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

	void span_list::Erase(span* pos)
	{
		assert(pos);
		assert(pos != m_Head);

		span* prev = pos->m_Prev;
		span* next = pos->m_Next;

		prev->m_Next = next;
		next->m_Prev = prev;
	}
}