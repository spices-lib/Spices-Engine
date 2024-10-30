#include "Pchheader.h"
#include "ConcurrentAlloc.h"
#include "ThrealCache.h"
#include "PageCache.h"
#include "ObjectPool.h"

namespace Spices {

	void* ConcurrentAlloc(size_t size)
	{
		if (size > MemoryHelper::MAX_BYTES)
		{
			size_t alignSize = MemoryHelper::AlignUp(size);
			size_t k = alignSize >> MemoryHelper::PAGE_SHIFT;

			PageCache::Get()->GetMutex().lock();
			span* s = PageCache::Get()->NewSpan(k);
			PageCache::Get()->GetMutex().unlock();

			void* ptr = (void*)(s->m_PageId >> MemoryHelper::PAGE_SHIFT);
			return ptr;
		}
		else
		{
			if (!pTLSThreadCache)
			{
				static ObjectPool<ThreadCache> objectPool;
				objectPool.GetMutex().lock();
				pTLSThreadCache = objectPool.New();
				objectPool.GetMutex().unlock();
			}

			return pTLSThreadCache->Allocate(size);
		}
	}

	void ConcurrentFree(void* ptr)
	{
		assert(ptr);

		span* s = PageCache::Get()->MapObjectToSpan(ptr);
		size_t size = s->m_ObjSize;

		if (size > MemoryHelper::MAX_BYTES)
		{
			span* s = PageCache::Get()->MapObjectToSpan(ptr);

			PageCache::Get()->GetMutex().lock();
			PageCache::Get()->ReleaseSpanToPageCache(s);
			PageCache::Get()->GetMutex().unlock();
		}
		else
		{
			pTLSThreadCache->Deallocate(ptr, size);
		}
	}
}