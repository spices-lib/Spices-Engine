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
		SPICES_PROFILE_ZONE;

		/**
		* @brief Only allowed allocate 258KB one tme.
		*/
		assert(size <= MemoryHelper::MAX_BYTES);

		/**
		* @brief Determain aligned size and freelist index.
		*/
		size_t alignSize = MemoryHelper::AlignUp(size);
		size_t index     = MemoryHelper::Index(size);

		/**
		* @brief Fetch memory from freelist.
		*/
		if (!m_FreeLists[index].Empty())
		{
			return m_FreeLists[index].Pop();
		}

		/**
		* @brief Fetch memory from cc;
		*/
		else
		{
			return FetchFromCentralCache(index, alignSize);
		}
	}

	void ThreadCache::Deallocate(void* obj, size_t size)
	{
		SPICES_PROFILE_ZONE;

		assert(obj);
		assert(size <= MemoryHelper::MAX_BYTES);

		/**
		* @brief Push object memory to free list.
		*/
		size_t index = MemoryHelper::Index(size);
		m_FreeLists[index].Push(obj);

		/**
		* @brief Release memory to cc.
		*/
		if (m_FreeLists[index].Size() >= m_FreeLists[index].ApplyforNBlocks())
		{
			ListTooLong(m_FreeLists[index], size);
		}
	}

	void* ThreadCache::FetchFromCentralCache(size_t index, size_t alignSize)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Slow-Start Threshold Dynamic Adjustment Algorithm.
		*/
		size_t batchNum = std::min(m_FreeLists[index].ApplyforNBlocks(), MemoryHelper::GetNBlocksLimit(alignSize));

		if (batchNum == m_FreeLists[index].ApplyforNBlocks())
		{
			m_FreeLists[index].IncreaseInNextApplyFor();
		}
		
		void* start = nullptr;
		void* end   = nullptr;

		/**
		* @brief Obtain actural blocks form cc.
		*/
		size_t actualNum = CenteralCache::Get()->FetchRange(start, end, batchNum, alignSize);

		assert(actualNum >= 1);

		/**
		* @brief push other blocks to freelist if obtained more than one block.
		*/
		if (actualNum > 1)
		{
			m_FreeLists[index].PushRange(MemoryHelper::PointerSpace(start), end, actualNum - 1);
		}

		return start;
	}

	void ThreadCache::ListTooLong(scl::free_list& list, size_t size)
	{
		SPICES_PROFILE_ZONE;

		void* start = nullptr;
		void* end   = nullptr;

		list.PopRange(start, end, list.ApplyforNBlocks());

		CenteralCache::Get()->ReleaseListToSpans(start, size);
	}

}