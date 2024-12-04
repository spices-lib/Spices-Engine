/**
* @file MemoryPool.h
* @brief The MemoryPool Class Definitions.
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
	* @brief MemoryPool Class.
	*/
	class MemoryPool
	{
	public:

		/**
		* @brief Number of freelist.
		* equals to 16 + 56 + 56 + 56 + 24
		*/
		static constexpr size_t FREE_LIST_NUM = 208;

		/**
		* @brief Only allowed 256KB allocated memory one time in tc.
		*/
		static constexpr size_t MAX_BYTES = 256 * 1024;

		/**
		* @brief number of pages in pc (1M).
		*/
		static constexpr size_t PAGE_NUM = 129;

		/**
		* @brief 8KB in a page.
		*/
		static constexpr size_t PAGE_SHIFT = 13;

		/**
		* @brief Alloc memory entry point.
		* @param[in] size memory bytes.
		* @return Returns memory pointer.
		*/
		static void* Alloc(size_t size);

		/**
		* @brief Free memory entry point.
		* @param[in] ptr memory pointer.
		*/
		static void Free(void* ptr);

		/**
		* @brief Get object first 4/8 bytes as a pointer.
		* @param[in] obj object pointer.
		* @return Returns object first 4/8 bytes.
		*/
		static void*& PointerSpace(void* obj);

		/**
		* @brief Align up memory bytes.
		* @param[in] size memory block bytes.
		* @return Returns Align up size.
		*/
		static size_t AlignUp(size_t size);

		/**
		* @brief Determine which freelist should process the memory block with given bytes.
		* @param[in] size memory block bytes.
		* @return Returns index of freelist.
		*/
		static size_t Index(size_t size);

		/**
		* @brief Determine how much bytes freelist should process the memory block with given index.
		* @param[in] index freelist index.
		* @return Returns bytes of freelist index.
		*/
		static size_t Bytes(size_t index);

		/**
		* @brief Get count of blocks limit by tc align up bytes.
		* @param[in] size align up bytes.
		* @return Returns blocks limit.
		*/
		static size_t GetNBlocksLimit(size_t size);

		/**
		* @brief Get pages count by given bytes.
		* @param[in] size bytes.
		* @return Returns pages count
		*/
		static size_t GetPages(size_t size);
	};

	/**
	* @brief Alloc memory from system.
	* @param[in] kpage pages.
	* @return Returns allocated memory pointer.
	*/
	inline static void* SystemAlloc(size_t kpage)
	{
#ifdef _WIN32

		/**
		* @brief alloc bytes = kpage * 8KB.
		*/
		void* ptr = VirtualAlloc(0, kpage << MemoryPool::PAGE_SHIFT, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else

#endif

		if (ptr == nullptr)
		{
			SPICES_CORE_ERROR("Memory alloc failed.")
		}

		SPICES_PROFILE_ALLOC_N(ptr, kpage << MemoryPool::PAGE_SHIFT, memoryPoolNames[1]);

		return ptr;
	}

	/**
	* @brief Free memory to system.
	* @param[in] ptr memory pointer.
	*/
	inline static void SystemFree(void* ptr)
	{
		SPICES_PROFILE_FREE_N(ptr, memoryPoolNames[1]);

#ifdef _WIN32

		VirtualFree(ptr, 0, MEM_RELEASE);
#else

#endif
	}
}