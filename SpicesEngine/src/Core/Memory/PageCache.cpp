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
		std::unique_lock<std::mutex> lock(m_Mutex);

		return InternalNewSpan(k);
	}

	scl::span* PageCache::MapObjectToSpan(void* obj) const
	{
		/**
		* @brief Get page id by memory.
		*/
		const size_t id = (reinterpret_cast<size_t>(obj) >> MemoryPool::PAGE_SHIFT);

		/**
		* @brief Find span in map.
		*/
		void* s = m_IdSpanMap.get(id);

		if (s)
		{
			return static_cast<scl::span*>(s);
		}
		else
		{
			return nullptr;
		}
	}
	
	void PageCache::ReleaseSpanToPageCache(scl::span* s)
	{
		std::unique_lock<std::mutex> lock(m_Mutex);

		/**
		* @brief Release to system.
		*/
		if (s->m_NPages > MemoryPool::PAGE_NUM - 1)
		{
			void* ptr = reinterpret_cast<void*>(s->m_PageId << MemoryPool::PAGE_SHIFT);
			SystemFree(ptr);

			m_IdSpanMap.set(s->m_PageId, nullptr);
			m_SpanPool.Delete(s);
			
			return;
		}

		/**
		* @brief Merge to left.
		*/
		for(;;)
		{
			const size_t leftId = s->m_PageId - 1;

			scl::span* leftSpan = static_cast<scl::span*>(m_IdSpanMap.get(leftId));

			if (!leftSpan)
			{
				break;
			}

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
		for (;;)
		{
			const size_t rightId = s->m_PageId + s->m_NPages;

			scl::span* rightSpan = static_cast<scl::span*>(m_IdSpanMap.get(rightId));

			if (!rightSpan)
			{
				break;
			}

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

		m_IdSpanMap.set(s->m_PageId, s);
		m_IdSpanMap.set(s->m_PageId + s->m_NPages - 1, s);
	}

	scl::span* PageCache::InternalNewSpan(size_t k)
	{
		assert(k > 0);

		/**
		* @brief Allocate from system.
		*/
		if (k > MemoryPool::PAGE_NUM - 1)
		{
			void* ptr      = SystemAlloc(k);
			scl::span* s   = m_SpanPool.New();
			s->m_PageId    = reinterpret_cast<size_t>(ptr) >> MemoryPool::PAGE_SHIFT;
			s->m_NPages    = k;
			s->m_BlockSize = k * (1 << MemoryPool::PAGE_SHIFT);

			m_IdSpanMap.set(s->m_PageId, s);

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
			scl::span* s   = m_SpanLists[k].PopFront();
			s->m_BlockSize = k * (1 << MemoryPool::PAGE_SHIFT);

			for (size_t i = 0; i < s->m_NPages; ++i)
			{
				m_IdSpanMap.set(s->m_PageId + i, s);
			}

			return s;
		}

		/**
		* @brief Iter begger span, try find spare pages in pc.
		*/
		for (size_t i = k + 1; i < MemoryPool::PAGE_NUM; ++i)
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

				m_IdSpanMap.set(nSpan->m_PageId, nSpan);
				m_IdSpanMap.set(nSpan->m_PageId + nSpan->m_NPages - 1, nSpan);

				for (size_t j = 0; j < kSpan->m_NPages; ++j)
				{
					m_IdSpanMap.set(kSpan->m_PageId + j, kSpan);
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
		bigSpan->m_PageId    = reinterpret_cast<size_t>(ptr) >> MemoryPool::PAGE_SHIFT;
		bigSpan->m_NPages    = MemoryPool::PAGE_NUM - 1;
		bigSpan->m_BlockSize = (1 << MemoryPool::PAGE_SHIFT) * bigSpan->m_NPages;

		/**
		* @brief Push span to list.
		*/
		m_SpanLists[MemoryPool::PAGE_NUM - 1].PushFront(bigSpan);

		return InternalNewSpan(k);
	}
}