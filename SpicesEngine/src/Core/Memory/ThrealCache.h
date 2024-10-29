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

	class ThreadCache
	{
	public:

		/**
		* @brief Alloc memory in this thread.
		* @param[in] size memory size.
		* @return Returns memory pointer.
		*/
		void* Allocate(size_t size);

		/**
		* @brief Recycle object memory.
		* @param[in] obj object pointer.
		* @param[in] size object bytes.
		*/
		void Deallocate(void* obj, size_t size);
		
		/**
		* @brief Fetch memory from central cache if this is run out.
		*/
		void* FetchFromCentralCache(size_t index, size_t alignSize);

		void ListTooLong(free_list& list, size_t size);

	private:

		/**
		* @brief FreeList Array.
		*/
		std::array<free_list, MemoryHelper::FREE_LIST_NUM> m_FreeLists;
	};

	/**
	* @brief Thread Unique variable of TC.
	*/
	static _declspec(thread) ThreadCache* pTLSThreadCache = nullptr;
}