#include "Pchheader.h"
#include "PageCache.h"

namespace Spices {

	PageCache PageCache::m_PageCache;

	scl::span* PageCache::NewSpan(size_t k)
	{
		assert(k > 0);

		if (k > MemoryHelper::PAGE_NUM - 1)
		{
			void* ptr = SystemAlloc(k);
			scl::span* s = m_SpanPool.New();

			return s;
		}

		if (!m_SpanLists[k].Empty())
		{
			scl::span* s = m_SpanLists[k].PopFront();

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
				scl::span* nSpan = m_SpanLists[i].PopFront();

				scl::span* kSpan = m_SpanPool.New();

				kSpan->m_PageId = nSpan->m_PageId;
				kSpan->n = k;

				nSpan->m_PageId += k;
				kSpan->n -= k;

				m_SpanLists[nSpan->n].PushFront(nSpan);

				m_IdSpanMap[nSpan->m_PageId] = nSpan;
				m_IdSpanMap[nSpan->m_PageId + nSpan->n - 1] = nSpan;

				for (size_t i = 0; i < kSpan->n; ++i)
				{
					m_IdSpanMap[kSpan->m_PageId + i] = kSpan;
				}

				return kSpan;
			}
		}

		void* ptr = SystemAlloc(MemoryHelper::PAGE_NUM - 1);

		scl::span* bigSpan = m_SpanPool.New();

		bigSpan->m_PageId = ((size_t)ptr) >> MemoryHelper::PAGE_SHIFT;
		bigSpan->n = MemoryHelper::PAGE_NUM - 1;

		m_SpanLists[MemoryHelper::PAGE_NUM - 1].PushFront(bigSpan);

		return NewSpan(k);
	}

	scl::span* PageCache::MapObjectToSpan(void* obj)
	{
		size_t id = (((size_t)obj) >> MemoryHelper::PAGE_SHIFT);

		std::unique_lock<std::mutex> ul(m_Mutex);

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

	void PageCache::ReleaseSpanToPageCache(scl::span* s)
	{
		if (s->n > MemoryHelper::PAGE_NUM - 1)
		{
			void* ptr = (void*)(s->m_PageId << MemoryHelper::PAGE_SHIFT);
			SystemFree(ptr);
			m_SpanPool.Delete(s);

			return;
		}

		while (1)
		{
			size_t leftId = s->m_PageId - 1;
			auto ret = m_IdSpanMap.find(leftId);

			if (ret == m_IdSpanMap.end())
			{
				break;
			}

			scl::span* leftSpan = ret->second;

			if (leftSpan->m_IsUse)
			{
				break;
			}

			if (leftSpan->n + s->n > MemoryHelper::PAGE_NUM - 1)
			{
				break;
			}

			s->m_PageId = leftSpan->m_PageId;
			s->n += leftSpan->n;

			m_SpanLists[leftSpan->n].Erase(leftSpan);
			m_SpanPool.Delete(leftSpan);
		}

		while (1)
		{
			size_t rightId = s->m_PageId + s->n;
			auto it = m_IdSpanMap.find(rightId);

			if (it == m_IdSpanMap.end())
			{
				break;
			}

			scl::span* rightSpan = it->second;
			if (rightSpan->m_IsUse)
			{
				break;
			}

			if (rightSpan->n + s->n > MemoryHelper::PAGE_NUM - 1)
			{
				break;
			}

			s->n += rightSpan->n;

			m_SpanLists[rightSpan->n].Erase(rightSpan);
			m_SpanPool.Delete(rightSpan);
		}

		m_SpanLists[s->n].PushFront(s);
		s->m_IsUse = false;

		m_IdSpanMap[s->m_PageId] = s;
		m_IdSpanMap[s->m_PageId + s->n - 1] = s;
	}

}