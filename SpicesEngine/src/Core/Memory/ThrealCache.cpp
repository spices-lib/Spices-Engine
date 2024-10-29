/**
* @file ThrealCache.cpp.
* @brief The ThrealCache Class Implementation.
* @author tcmalloc.
*/

#include "Pchheader.h"
#include "ThrealCache.h"
#include "CenteralCache.h"

namespace Spices {

	void* ThreadCache::Allocate(size_t size)
	{
		assert(size <= MemoryHelper::MAX_BYTES);

		size_t alignSize = MemoryHelper::AlignUp(size);
		size_t index = MemoryHelper::Index(size);

		if (!m_FreeLists[index].Empty())
		{
			return m_FreeLists[index].Pop();
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
		m_FreeLists[index].Push(obj);
	}

	void* ThreadCache::FetchFromCentralCache(size_t index, size_t alignSize)
	{
		size_t batchNum = std::min(m_FreeLists[index].MaxSize(), MemoryHelper::NumMoveSize(alignSize));

		if (batchNum == m_FreeLists[index].MaxSize())
		{
			m_FreeLists[index].MaxSize()++;
		}
		
		void* start = nullptr;
		void* end = nullptr;

		size_t actualNum = CenteralCache::Get()->FetchRangeObj(start, end, batchNum, alignSize);

		assert(actualNum >= 1);

		if (actualNum == 1)
		{
			assert(start == end);
			return start;
		}
		else
		{
			m_FreeLists[index].PushRange(MemoryHelper::ObjNext(start), end, actualNum - 1);
			return start;
		}
	}

}