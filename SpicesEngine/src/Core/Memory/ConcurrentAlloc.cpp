#include "Pchheader.h"
#include "ConcurrentAlloc.h"
#include "ThrealCache.h"
#include "PageCache.h"
#include "ObjectPool.h"

namespace Spices {

	void* ConcurrentAlloc(size_t size)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief allocate from pc.
		*/
		if (size > MemoryHelper::MAX_BYTES)
		{
			size_t alignSize = MemoryHelper::AlignUp(size);
			size_t k = alignSize >> MemoryHelper::PAGE_SHIFT;

			scl::span* s = PageCache::Get()->NewSpan(k);

			void* ptr = (void*)(s->m_PageId >> MemoryHelper::PAGE_SHIFT);
			return ptr;
		}

		/**
		* @brief allocate from tc.
		*/
		else
		{
			if (!pTLSThreadCache)
			{
				static ObjectPool<ThreadCache> objectPool;

				pTLSThreadCache = objectPool.ThreadNew();
			}

			return pTLSThreadCache->Allocate(size);
		}
	}

	void ConcurrentFree(void* ptr)
	{
		SPICES_PROFILE_ZONE;

		assert(ptr);

		scl::span* s = PageCache::Get()->MapObjectToSpan(ptr);
		size_t size = s->m_BlockSize;

		/**
		* @brief release from pc.
		*/
		if (size > MemoryHelper::MAX_BYTES)
		{
			scl::span* s = PageCache::Get()->MapObjectToSpan(ptr);

			PageCache::Get()->ReleaseSpanToPageCache(s);
		}

		/**
		* @brief release from tc.
		*/
		else
		{
			pTLSThreadCache->Deallocate(ptr, size);
		}
	}
}