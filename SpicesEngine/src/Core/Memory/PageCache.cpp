/**
* @file PageCache.cpp.
* @brief The PageCache Class Implementation.
* @author tcmalloc.
*/

#include "Pchheader.h"
#include "PageCache.h"

namespace Spices {

	PageCache PageCache::m_PageCache;

	scl::span* PageCache::NewSpan(size_t k)
	{
		SPICES_PROFILE_ZONE;

		std::unique_lock<std::mutex> lock(m_Mutex);

		return InternalNewSpan(k);
	}

	scl::span* PageCache::MapObjectToSpan(void* obj)
	{
		SPICES_PROFILE_ZONE;

		std::unique_lock<std::mutex> lock(m_Mutex);

		/**
		* @brief Get page id by memory.
		*/
		size_t id = (((size_t)obj) >> MemoryPool::PAGE_SHIFT);

		/**
		* @brief Find span in map.
		*/
		if (m_IdSpanMap.find(id) != m_IdSpanMap.end())
		{
			return m_IdSpanMap[id];
		}
		else
		{
			assert(false);
			return nullptr;
		}
	}
	
	void PageCache::ReleaseSpanToPageCache(scl::span* s)
	{
		SPICES_PROFILE_ZONE;

		std::unique_lock<std::mutex> lock(m_Mutex);

		/**
		* @brief Release to system.
		*/
		if (s->m_NPages > MemoryPool::PAGE_NUM - 1)
		{
			void* ptr = (void*)(s->m_PageId << MemoryPool::PAGE_SHIFT);
			SystemFree(ptr);
			m_SpanPool.Delete(s);

			return;
		}

		/**
		* @brief Merge to left.
		*/
		while (1)
		{
			size_t leftId = s->m_PageId - 1;

			if (m_IdSpanMap.find(leftId) == m_IdSpanMap.end())
			{
				break;
			}

			scl::span* leftSpan = m_IdSpanMap[leftId];

			if (leftSpan->m_IsUse)
			{
				break;
			}

			if (leftSpan->m_NPages + s->m_NPages > MemoryPool::PAGE_NUM - 1)
			{
				break;
			}

			s->m_PageId  = leftSpan->m_PageId;
			s->m_NPages += leftSpan->m_NPages;

			m_SpanLists[leftSpan->m_NPages].Erase(leftSpan);
			m_SpanPool.Delete(leftSpan);
		}

		/**
		* @brief Merge to right.
		*/
		while (1)
		{
			size_t rightId = s->m_PageId + s->m_NPages;

			if (m_IdSpanMap.find(rightId) == m_IdSpanMap.end())
			{
				break;
			}

			scl::span* rightSpan = m_IdSpanMap[rightId];

			if (rightSpan->m_IsUse)
			{
				break;
			}

			if (rightSpan->m_NPages + s->m_NPages > MemoryPool::PAGE_NUM - 1)
			{
				break;
			}

			s->m_NPages += rightSpan->m_NPages;

			m_SpanLists[rightSpan->m_NPages].Erase(rightSpan);
			m_SpanPool.Delete(rightSpan);
		}

		/**
		* @brief Push span to list.
		*/
		m_SpanLists[s->m_NPages].PushFront(s);
		s->m_IsUse = false;

		m_IdSpanMap[s->m_PageId] = s;
		m_IdSpanMap[s->m_PageId + s->m_NPages - 1] = s;
	}

	scl::span* PageCache::InternalNewSpan(size_t k)
	{
		SPICES_PROFILE_ZONE;

		assert(k > 0);

		/**
		* @brief Allocate from system.
		*/
		if (k > MemoryPool::PAGE_NUM - 1)
		{
			void* ptr      = SystemAlloc(k);
			scl::span* s   = m_SpanPool.New();
			s->m_PageId    = ((size_t)ptr) >> MemoryPool::PAGE_SHIFT;
			s->m_NPages    = k;
			s->m_BlockSize = k * (1 << MemoryPool::PAGE_SHIFT);

			m_IdSpanMap[s->m_PageId] = s;

			return s;
		}

		/**
		* @brief Has empty pages in pc.
		*/
		if (!m_SpanLists[k].Empty())
		{
			/**
			* @brief Pop a span.
			*/
			scl::span* s = m_SpanLists[k].PopFront();

			for (size_t i = 0; i < s->m_NPages; ++i)
			{
				m_IdSpanMap[s->m_PageId + i] = s;
			}

			return s;
		}

		/**
		* @brief Iter begger span, try find spare pages in pc.
		*/
		for (int i = k + 1; i < MemoryPool::PAGE_NUM; ++i)
		{
			if (!m_SpanLists[i].Empty())
			{
				/**
				* @brief Pop a span.
				*/
				scl::span* nSpan   = m_SpanLists[i].PopFront();

				/**
				* @brief New a span to split bigger span.
				*/
				scl::span* kSpan   = m_SpanPool.New();
				kSpan->m_PageId    = nSpan->m_PageId;
				kSpan->m_NPages    = k;
				kSpan->m_BlockSize = (1 << MemoryPool::PAGE_SHIFT) * k;

				nSpan->m_PageId   += k;
				nSpan->m_NPages   -= k;
				nSpan->m_BlockSize = (1 << MemoryPool::PAGE_SHIFT) * nSpan->m_NPages;

				/**
				* @brief Push splited span to list.
				*/
				m_SpanLists[nSpan->m_NPages].PushFront(nSpan);

				m_IdSpanMap[nSpan->m_PageId] = nSpan;
				m_IdSpanMap[nSpan->m_PageId + nSpan->m_NPages - 1] = nSpan;

				for (size_t i = 0; i < kSpan->m_NPages; ++i)
				{
					m_IdSpanMap[kSpan->m_PageId + i] = kSpan;
				}

				return kSpan;
			}
		}

		/**
		* @brief Allocate memory from system if no spare pages in pc.
		*/
		void* ptr = SystemAlloc(MemoryPool::PAGE_NUM - 1);

		/**
		* @brief New a span to mamage this memory.
		*/
		scl::span* bigSpan   = m_SpanPool.New();
		bigSpan->m_PageId    = ((size_t)ptr) >> MemoryPool::PAGE_SHIFT;
		bigSpan->m_NPages    = MemoryPool::PAGE_NUM - 1;
		bigSpan->m_BlockSize = (1 << MemoryPool::PAGE_SHIFT) * bigSpan->m_NPages;

		/**
		* @brief Push span to list.
		*/
		m_SpanLists[MemoryPool::PAGE_NUM - 1].PushFront(bigSpan);

		return InternalNewSpan(k);
	}
}