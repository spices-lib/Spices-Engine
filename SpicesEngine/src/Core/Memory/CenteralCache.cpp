#include "Pchheader.h"
#include "CenteralCache.h"
#include "PageCache.h"

namespace Spices {

	CenteralCache CenteralCache::m_CenteralCache;

	size_t CenteralCache::FetchRangeObj(void*& start, void*& end, size_t batchNum, size_t size)
	{
		SPICES_PROFILE_ZONE;

		size_t index = MemoryHelper::Index(size);

		m_SpanLists[index].GetMutex().lock();

		span* s = GetOneSpan(m_SpanLists[index], size);
		assert(s);
		assert(s->m_FreeList);

		start = end = s->m_FreeList;
		size_t acturalNum = 1;

		size_t i = 0;
		while (i < batchNum - 1 && MemoryHelper::ObjNext(end) != nullptr)
		{
			end = MemoryHelper::ObjNext(end);
			++acturalNum;
			++i;
		}

		s->m_FreeList = MemoryHelper::ObjNext(end);
		s->m_UseCount += acturalNum;
		MemoryHelper::ObjNext(end) = nullptr;

		m_SpanLists[index].GetMutex().unlock();

		return acturalNum;
	}

	span* CenteralCache::GetOneSpan(span_list& list, size_t size)
	{
		span* it = list.Begin();
		while (it != list.End())
		{
			if (it->m_FreeList != nullptr)
			{
				return it;
			}
			else
			{
				it = it->m_Next;
			}
		}

		list.GetMutex().unlock();

		size_t k = MemoryHelper::NumMovePage(size);
		PageCache::Get()->GetMutex().lock();
		span* s = PageCache::Get()->NewSpan(k);
		s->m_IsUse = true;
		s->m_ObjSize = size;
		PageCache::Get()->GetMutex().unlock();

		char* start = (char*)(s->m_PageId << MemoryHelper::PAGE_SHIFT);
		char* end = (char*)(start + (s->n << MemoryHelper::PAGE_SHIFT));

		s->m_FreeList = start;

		void* tail = start;
		start += size;

		int i = 0;
		while (start < end)
		{
			++i;
			MemoryHelper::ObjNext(tail) = start;
			start += size;
			tail = MemoryHelper::ObjNext(tail);
		}
		MemoryHelper::ObjNext(tail) = nullptr;

		list.GetMutex().lock();
		list.PushFront(s);

		return s;
	}

	void CenteralCache::ReleaseListToSpans(void* start, size_t size)
	{
		size_t index = MemoryHelper::Index(size);

		m_SpanLists[index].GetMutex().lock();

		while (start)
		{
			void* next = MemoryHelper::ObjNext(start);

			span* s = PageCache::Get()->MapObjectToSpan(start);

			MemoryHelper::ObjNext(start) = s->m_FreeList;
			s->m_FreeList = start;

			s->m_UseCount--;
			if (s->m_UseCount == 0)
			{
				m_SpanLists[index].Erase(s);
				s->m_FreeList = nullptr;
				s->m_Next = nullptr;
				s->m_Prev = nullptr;

				m_SpanLists[index].GetMutex().unlock();

				PageCache::Get()->GetMutex().lock();
				PageCache::Get()->ReleaseSpanToPageCache(s);
				PageCache::Get()->GetMutex().unlock();

				m_SpanLists[index].GetMutex().lock();
			}

			start = next;
		}

		m_SpanLists[index].GetMutex().unlock();
	}
}