#include "Pchheader.h"
#include "PageCache.h"

namespace Spices {

	PageCache PageCache::m_PageCache;

	span* PageCache::NewSpan(size_t k)
	{
		assert(k > 0 && k < MemoryHelper::PAGE_NUM);

		if (!m_SpanLists[k].Empty())
		{
			span* s = m_SpanLists[k].PopFront();

			for (size_t i = 0; i < s->n; ++i)
			{
				m_IdSpanMap[s->m_PageId + i] = s;
			}

			return s;
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

				for (size_t i = 0; i < kSpan->n; ++i)
				{
					m_IdSpanMap[kSpan->m_PageId + i] = kSpan;
				}

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

	span* PageCache::MapObjectToSpan(void* obj)
	{
		size_t id = (((size_t)obj) >> MemoryHelper::PAGE_SHIFT);

		auto ret = m_IdSpanMap.find(id);

		if (ret != m_IdSpanMap.end())
		{
			return ret->second;
		}
		else
		{
			assert(false);
			return nullptr;
		}
	}

	void PageCache::ReleaseSpanToPageCache(span* s)
	{

	}

}