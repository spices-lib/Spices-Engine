#include "Pchheader.h"
#include "CenteralCache.h"
#include "PageCache.h"

namespace Spices {

	CenteralCache CenteralCache::m_CenteralCache;

	size_t CenteralCache::FetchRangeObj(void*& start, void*& end, size_t batchNum, size_t size)
	{
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
		PageCache::Get()->GetMutex().unlock();

		char* start = (char*)(s->m_PageId << MemoryHelper::PAGE_SHIFT);
		char* end = (char*)(start + (s->n << MemoryHelper::PAGE_SHIFT));

		s->m_FreeList = start;

		void* tail = start;
		start += size;

		while (start < end)
		{
			MemoryHelper::ObjNext(tail) = start;
			start += size;
			tail = MemoryHelper::ObjNext(tail);
		}
		MemoryHelper::ObjNext(tail) = nullptr;

		list.GetMutex().lock();
		list.PushFront(s);

		return s;
	}
}