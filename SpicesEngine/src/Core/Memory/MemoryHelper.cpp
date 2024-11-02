#include "Pchheader.h"
#include "MemoryHelper.h"
#include "Core/Library/MemoryLibrary.h"

namespace Spices {

	void*& MemoryHelper::PointerSpace(void* obj)
	{
		SPICES_PROFILE_ZONE;

		return *(void**)obj;
	}

	size_t MemoryHelper::AlignUp(size_t size)
	{
		SPICES_PROFILE_ZONE;

		assert(size <= MAX_BYTES);

		if      (size <= 128)          return MemoryLibrary::align_up<size_t>(size, 8);                              /* @brief align up to 8B  , if size is less than 128B. (16)  */
		else if (size <= 1 *   1024)   return MemoryLibrary::align_up<size_t>(size, 16);                             /* @brief align up to 16B , if size is less than 1KB.  (56)  */
		else if (size <= 8   * 1024)   return MemoryLibrary::align_up<size_t>(size, 128);                            /* @brief align up to 128B, if size is less than 8KB.  (56)  */
		else if (size <= 64  * 1024)   return MemoryLibrary::align_up<size_t>(size, 1024);                           /* @brief align up to 1KB , if size is less than 64KB. (56)  */
		else if (size <= 256 * 1024)   return MemoryLibrary::align_up<size_t>(size, 8 * 1024);                       /* @brief align up to 8KB , if size is less than 256KB.(24)  */
		else                           return MemoryLibrary::align_up<size_t>(size, 1 << MemoryHelper::PAGE_SHIFT);  /* @brief align up to page                                   */
	}

	size_t MemoryHelper::Index(size_t size)
	{
		SPICES_PROFILE_ZONE;

		assert(size <= MAX_BYTES);

		auto _index = [&](size_t size, size_t align_shift) {
			return ((size + (1 << align_shift) - 1) >> align_shift) - 1;
		};

		static constexpr int group_array[4] = { 16, 56, 56, 56 };

		if (size <= 128)
		{
			return _index(size, 3);
		}
		else if (size <= 1024)
		{
			return _index(size - 128, 4) + 
				group_array[0];
		}
		else if (size <= 8 * 1024)
		{
			return _index(size - 1024, 7) + 
				group_array[1] + 
				group_array[0];
		}
		else if (size <= 64 * 1024)
		{
			return _index(size - 8 * 1024, 10) + 
				group_array[2] + 
				group_array[1] + 
				group_array[0];
		}
		else if (size <= 256 * 1024)
		{
			return _index(size - 64 * 1024, 13) + 
				group_array[3] + 
				group_array[2] + 
				group_array[1] + 
				group_array[0];
		}
		else
		{
			assert(false);
			return -1;
		}
	}

	size_t MemoryHelper::GetNBlocksLimit(size_t size)
	{
		SPICES_PROFILE_ZONE;

		assert(size > 0);

		/**
		* @brief [2 - 512].
		*/
		int num = MAX_BYTES / size;
		num = std::max(std::min(512, num), 2);

		return num;
	}

	size_t MemoryHelper::GetPages(size_t size)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief get blocks count.
		*/
		size_t num = GetNBlocksLimit(size);

		/**
		* @brief get pages count.
		*/
		size_t npage = num * size;
		npage >>= PAGE_SHIFT;
		npage = std::min(npage, static_cast<size_t>(1));

		return npage;
	}
}