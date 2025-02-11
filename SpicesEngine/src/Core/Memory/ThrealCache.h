/**
* @file ThrealCache.cpp.
* @brief The ThreadCache Class Definitions.
* @author tcmalloc.
*/

#pragma once
#include "Core/Core.h"
#include "MemoryPool.h"
#include "Core/Container/FreeList.h"

namespace Spices {

	/**
	* @brief Thread memory cache.
	* First level of memory allocator.
	*/
	class ThreadCache
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		ThreadCache() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~ThreadCache();

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaves.
		*/
		ThreadCache(const ThreadCache&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaves.
		*/
		ThreadCache& operator=(const ThreadCache&) = delete;

		/**
		* @brief Allocate memory.
		* @param[in] size memory bytes.
		* @return Returns memory pointer.
		*/
		void* Allocate(size_t size);

		/**
		* @brief Recycle object memory.
		* @param[in] obj object pointer.
		* @param[in] size object bytes.
		*/
		void Deallocate(void* obj, size_t size);
		
	private:

		/**
		* @brief Release memory to cc.
		* @param[in] list free_list.
		* @param[in] size object bytes.
		* @param[in] count release count.
		*/
		static void ReleaseToCentralCache(scl::free_list& list, size_t size, size_t count);

		/**
		* @brief Fetch memory from central cache if this is run out.
		* @param[in] index freelist index.
		* @param[in] alignSize align up bytes.
		* @return Returns memory pointer.
		*/
		void* FetchFromCentralCache(size_t index, size_t alignSize);

	private:

		/**
		* @brief FreeList Array.
		*/
		std::array<scl::free_list, MemoryPool::FREE_LIST_NUM> m_FreeLists;
	};

	/**
	* @brief Wapper of Instance/Delete ThreadCache in thread.
	*/
	class ThreadCacheThreadWapper
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		ThreadCacheThreadWapper() : instance(nullptr) {}

		/**
		* @brief Destructor Function.
		*/
		virtual ~ThreadCacheThreadWapper();

		/**
		* @brief Get ThreadCache Instance.
		* @reutrn Returns ThreadCache Instance.
		*/
		static ThreadCache*& GetInst();

	private:

		/**
		* @brief This thread ThreadCache instance.
		*/
		ThreadCache* instance;
	};
}