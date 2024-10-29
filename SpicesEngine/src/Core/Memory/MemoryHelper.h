/**
* @file MemoryHelper.h
* @brief The MemoryHelper Class Definitions.
* @author tcmalloc.
*/

#pragma once
#include "Core/Core.h"

#ifdef _WIN32
#include <Windows.h>
#else

#endif

namespace Spices {

	/**
	* @brief Alloc memory from system.
	* @param[in] kpage pages.
	* @return Returns alloced memory pointer.
	*/
	inline static void* SystemAlloc(size_t kpage)
	{
#ifdef _WIN32

		/**
		* @brief alloc bytes = kpage * 8KB.
		*/
		void* ptr = VirtualAlloc(0, kpage << 13, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else

#endif

		if (ptr == nullptr)
		{
			SPICES_CORE_ERROR("Memory alloc failed.");
		}

		return ptr;
	}

	class MemoryHelper
	{
	public:

		/**
		* @brief Number of freelist.
		*/
		static const size_t FREE_LIST_NUM = 208;

		/**
		* @brief Only allowed 256KB alloced memory one time.
		*/
		static const size_t MAX_BYTES = 256 * 1024;

		/**
		* @brief Get object address.
		*/
		static void*& ObjNext(void* obj);


		static size_t AlignUp(size_t size);

		/**
		* @brief Get index of freelist.
		* @param[in] size object butes.
		* @return Returns index of freelist.
		*/
		static size_t Index(size_t size);
	};
}