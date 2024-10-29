/**
* @file ThrealCache.cpp.
* @brief The ThrealCache Class Implementation.
* @author tcmalloc.
*/

#include "Pchheader.h"
#include "ThrealCache.h"

namespace Spices {

	void* ThreadCache::Allocate(size_t size)
	{
		assert(size <= MemoryHelper::MAX_BYTES);

		size_t alignSize = MemoryHelper::AlignUp(size);
		size_t index = MemoryHelper::Index(size);

		if (!m_FreeList[index].Empty())
		{
			return m_FreeList[index].Pop();
		}
		else
		{
			return FetchFromCentralCache(index, alignSize);
		}
	}

	void ThreadCache::Deallocate(void* obj, size_t size)
	{
		assert(obj);
		assert(size <= MemoryHelper::MAX_BYTES);

		size_t index = MemoryHelper::Index(size);
		m_FreeList[index].Push(obj);
	}

	void* ThreadCache::FetchFromCentralCache(size_t index, size_t alignSize)
	{
		return nullptr;
	}

}