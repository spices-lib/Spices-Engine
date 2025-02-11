/**
* @file ThrealCache.cpp.
* @brief The ThreadCache Class Implementation.
* @author tcmalloc.
*/

#include "Pchheader.h"
#include "ThrealCache.h"
#include "CentralCache.h"
#include "ObjectPool.h"

namespace Spices {

	ThreadCache::~ThreadCache()
	{
		/**
		* @brief Release all memory to cc while thread is destroied.
		*/
		for (int i = 0; i < MemoryPool::FREE_LIST_NUM; i++)
		{
			if (m_FreeLists[i].Size() > 0)
			{
				ReleaseToCentralCache(m_FreeLists[i], MemoryPool::Bytes(i), m_FreeLists[i].Size());
			}
		}
	}

	void* ThreadCache::Allocate(size_t size)
	{
		/**
		* @brief Only allowed allocate 258KB one tme.
		*/
		assert(size <= MemoryPool::MAX_BYTES);

		/**
		* @brief Determain aligned size and freelist index.
		*/
		const size_t alignSize = MemoryPool::AlignUp(size);
		const size_t index     = MemoryPool::Index(size);

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
		assert(obj);
		assert(size <= MemoryPool::MAX_BYTES);

		/**
		* @brief Push object memory to free list.
		*/
		const size_t index = MemoryPool::Index(size);
		m_FreeLists[index].Push(obj);

		/**
		* @brief Release memory to cc.
		*/
		if (m_FreeLists[index].Size() >= m_FreeLists[index].ApplyforNBlocks())
		{
			ReleaseToCentralCache(m_FreeLists[index], size, m_FreeLists[index].ApplyforNBlocks());
		}
	}

	void* ThreadCache::FetchFromCentralCache(size_t index, size_t alignSize)
	{
		/**
		* @brief Slow-Start Threshold Dynamic Adjustment Algorithm.
		*/
		const size_t batchNum = std::min(m_FreeLists[index].ApplyforNBlocks(), MemoryPool::GetNBlocksLimit(alignSize));

		if (batchNum == m_FreeLists[index].ApplyforNBlocks())
		{
			m_FreeLists[index].IncreaseInNextApplyFor();
		}
		
		void* start = nullptr;
		void* end   = nullptr;

		/**
		* @brief Obtain actural blocks form cc.
		*/
		const size_t actualNum = CentralCache::Get()->FetchRange(start, end, batchNum, alignSize);

		assert(actualNum >= 1);

		/**
		* @brief push other blocks to freelist if obtained more than one block.
		*/
		if (actualNum > 1)
		{
			m_FreeLists[index].PushRange(MemoryPool::PointerSpace(start), end, actualNum - 1);
		}

		return start;
	}

	void ThreadCache::ReleaseToCentralCache(scl::free_list& list, size_t size, size_t count)
	{
		void* start = nullptr;
		void* end   = nullptr;

		list.PopRange(start, end, count);

		CentralCache::Get()->ReleaseListToSpans(start, size);
	}

	/**
	* @brief ThreadCache ObjectPool for all threads.
	*/
	static ObjectPool<ThreadCache> objectPool;

	ThreadCacheThreadWapper::~ThreadCacheThreadWapper()
	{
		if (instance)
		{
			objectPool.ThreadDelete(instance);
		}
	}

	ThreadCache*& ThreadCacheThreadWapper::GetInst()
	{
		/**
		* @brief Thread Unique TCWapper.
		*/
		static _declspec(thread) ThreadCacheThreadWapper pTLSThreadCache;

		if (!pTLSThreadCache.instance)
		{
			pTLSThreadCache.instance = objectPool.ThreadNew();
		}

		return pTLSThreadCache.instance;
	}
}