/**
* @file CenteralCache.cpp.
* @brief The CenteralCache Class Implementation.
* @author tcmalloc.
*/

#include "Pchheader.h"
#include "CenteralCache.h"
#include "PageCache.h"

namespace Spices {

	CenteralCache CenteralCache::m_CenteralCache;

	size_t CenteralCache::FetchRange(void*& start, void*& end, size_t batchNum, size_t size)
	{
		SPICES_PROFILE_ZONE;

		size_t index = MemoryPool::Index(size);
		size_t acturalNum = 1;

		{
			std::unique_lock<std::mutex> lock(m_SpanLists[index].GetMutex());

			scl::span* s = GetOneSpan(m_SpanLists[index], size);
			assert(s);
			assert(s->m_FreeList);

			start = end = s->m_FreeList;
			
			/**
			* @brief fetch spare memory.
			*/
			size_t i = 0;
			while (i < batchNum - 1 && MemoryPool::PointerSpace(end) != nullptr)
			{
				end = MemoryPool::PointerSpace(end);
				++acturalNum;
				++i;
			}

			s->m_FreeList = MemoryPool::PointerSpace(end);
			s->m_UseCount += acturalNum;
			MemoryPool::PointerSpace(end) = nullptr;
		}

		return acturalNum;
	}

	scl::span* CenteralCache::GetOneSpan(scl::span_list& list, size_t size)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Find span in cc.
		*/
		scl::span* it = list.Begin();
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

		/**
		* @brief Release mutex.
		*/
		list.GetMutex().unlock();

		/**
		* @brief get pages count.
		*/
		size_t k = MemoryPool::GetPages(size);

		/**
		* @brief get a new span.
		*/
		scl::span* s   = PageCache::Get()->NewSpan(k);
		s->m_IsUse     = true;
		s->m_BlockSize = size;

		/**
		* @brief get start/end pointer.
		*/
		char* start = (char*)(         s->m_PageId << MemoryPool::PAGE_SHIFT );
		char* end   = (char*)(start + (s->m_NPages << MemoryPool::PAGE_SHIFT));

		s->m_FreeList = start;

		/**
		* @brief split pages to blocks.
		*/
		{
			void* tail = start;
			start += size;

			int i = 0;
			while (start < end)
			{
				++i;
				MemoryPool::PointerSpace(tail) = start;
				start += size;
				tail = MemoryPool::PointerSpace(tail);
			}
			MemoryPool::PointerSpace(tail) = nullptr;
		}

		/**
		* @brief Get mutex.
		*/
		list.GetMutex().lock();

		/**
		* @brief Push to list.
		*/
		list.PushFront(s);

		return s;
	}

	void CenteralCache::ReleaseListToSpans(void* start, size_t size)
	{
		SPICES_PROFILE_ZONE;

		size_t index = MemoryPool::Index(size);

		{
			std::unique_lock<std::mutex> lock(m_SpanLists[index].GetMutex());

			/**
			* @brief Iter all blocks and insert to span.
			*/
			while (start)
			{
				void* next = MemoryPool::PointerSpace(start);

				/**
				* @brief Find span.
				*/
				scl::span* s = PageCache::Get()->MapObjectToSpan(start);

				/**
				* @brief Insert from head to span.
				*/
				MemoryPool::PointerSpace(start) = s->m_FreeList;
				s->m_FreeList = start;
				s->m_UseCount--;

				/**
				* @brief if span is not in use, than release to pc.
				*/
				if (s->m_UseCount == 0)
				{
					m_SpanLists[index].Erase(s);
					s->m_FreeList = nullptr;
					s->m_Next     = nullptr;
					s->m_Prev     = nullptr;

					/**
					* @brief unlock span mutex.
					*/
					m_SpanLists[index].GetMutex().unlock();

					/**
					* @brief release to pc.
					*/
					PageCache::Get()->ReleaseSpanToPageCache(s);

					/**
					* @brief get span mutex.
					*/
					m_SpanLists[index].GetMutex().lock();
				}

				start = next;
			}
		}
	}
}