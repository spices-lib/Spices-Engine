/**
* @file ThrealCache.cpp.
* @brief The ThrealCache Class Definitions.
* @author tcmalloc.
*/

#pragma once
#include "Core/Core.h"
#include "MemoryHelper.h"
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
		virtual ~ThreadCache() = default;

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
		
		void ListTooLong(scl::free_list& list, size_t size);

	private:

		/**
		* @brief Fetch memory from central cache if this is run out.
		* @param[in] index freelist index.
		* @param[in] alignSize alignup bytes.
		* @return Returns memory pointer.
		*/
		void* FetchFromCentralCache(size_t index, size_t alignSize);

	private:

		/**
		* @brief FreeList Array.
		*/
		std::array<scl::free_list, MemoryHelper::FREE_LIST_NUM> m_FreeLists;
	};

	/**
	* @brief Thread Unique TC.
	*/
	static _declspec(thread) ThreadCache* pTLSThreadCache = nullptr;
}