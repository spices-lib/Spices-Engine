#include "Pchheader.h"
#include "PageCache.h"

namespace Spices {

	PageCache PageCache::m_PageCache;

	span* PageCache::NewSpan(size_t k)
	{
		assert(k > 0 && k < MemoryHelper::PAGE_NUM);

		if (!m_SpanLists[k].Empty())
		{
			return m_SpanLists[k].PopFront();
		}

		for (int i = k + 1; i < MemoryHelper::PAGE_NUM; ++i)
		{
			if (!m_SpanLists[i].Empty())
			{
				span* nSpan = m_SpanLists[i].PopFront();

				span* kSpan = new span;

				kSpan->m_PageId = nSpan->m_PageId;
				kSpan->n = k;

				nSpan->m_PageId += k;
				kSpan->n -= k;

				m_SpanLists[nSpan->n].PushFront(nSpan);
				return kSpan;
			}
		}

		void* ptr = SystemAlloc(MemoryHelper::PAGE_NUM - 1);

		span* bigSpan = new span;

		bigSpan->m_PageId = ((size_t)ptr) >> MemoryHelper::PAGE_SHIFT;
		bigSpan->n = MemoryHelper::PAGE_NUM - 1;

		m_SpanLists[MemoryHelper::PAGE_NUM - 1].PushFront(bigSpan);

		return NewSpan(k);
	}

}