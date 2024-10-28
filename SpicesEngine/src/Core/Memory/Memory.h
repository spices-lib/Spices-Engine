/**
* @file Memory.h
* @brief The Memory Functions Definitions.
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

	/**
	* @brief Get object address.
	*/
	static void*& ObjNext(void* obj)
	{
		return *(void**)obj;
	}

}