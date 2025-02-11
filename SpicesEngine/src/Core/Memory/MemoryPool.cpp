/**
* @file MemoryPool.h
* @brief The MemoryPool Class Implementation.
* @author tcmalloc.
*/

#include "Pchheader.h"
#include "MemoryPool.h"
#include "Core/Library/MemoryLibrary.h"
#include "PageCache.h"
#include "ThrealCache.h"

namespace Spices {

	void* MemoryPool::Alloc(size_t size)
	{
		/**
		* @brief allocate from pc.
		*/
		if (size > MAX_BYTES)
		{
			const size_t alignSize = AlignUp(size);
			size_t k = alignSize >> PAGE_SHIFT;

			scl::span* s   = PageCache::Get()->NewSpan(k);
			s->m_IsUse     = true;
			s->m_BlockSize = alignSize;

			void* ptr = reinterpret_cast<void*>(s->m_PageId << PAGE_SHIFT);
			return ptr;
		}

		/**
		* @brief allocate from tc.
		*/
		else
		{
			return ThreadCacheThreadWapper::GetInst()->Allocate(size);
		}
	}

	void MemoryPool::Free(void* ptr)
	{
		assert(ptr);

		scl::span* s = PageCache::Get()->MapObjectToSpan(ptr);

		if (!s)
		{
			free(ptr);
			return;
		}

		const size_t size = s->m_BlockSize;

		/**
		* @brief release from pc.
		*/
		if (size > MAX_BYTES)
		{
			PageCache::Get()->ReleaseSpanToPageCache(s);
		}

		/**
		* @brief release from tc.
		*/
		else
		{
			ThreadCacheThreadWapper::GetInst()->Deallocate(ptr, size);
		}
	}

	void*& MemoryPool::PointerSpace(void* obj)
	{
		return *(static_cast<void**>(obj));
	}

	size_t MemoryPool::AlignUp(size_t size)
	{
		if      (size <= static_cast<size_t>(       128))   return MemoryLibrary::align_up<size_t>(size, 8);                /* @brief align up to 8B  , if size is less than 128B. (16)  */
		else if (size <= static_cast<size_t>(1 *   1024))   return MemoryLibrary::align_up<size_t>(size, 16);               /* @brief align up to 16B , if size is less than 1KB.  (56)  */
		else if (size <= static_cast<size_t>(8   * 1024))   return MemoryLibrary::align_up<size_t>(size, 128);              /* @brief align up to 128B, if size is less than 8KB.  (56)  */
		else if (size <= static_cast<size_t>(64  * 1024))   return MemoryLibrary::align_up<size_t>(size, 1024);             /* @brief align up to 1KB , if size is less than 64KB. (56)  */
		else if (size <= static_cast<size_t>(256 * 1024))   return MemoryLibrary::align_up<size_t>(size, 8 * 1024);         /* @brief align up to 8KB , if size is less than 256KB.(24)  */
		else                                                return MemoryLibrary::align_up<size_t>(size, 1 << PAGE_SHIFT);  /* @brief align up to page                                   */
	}

	size_t MemoryPool::Index(size_t size)
	{
		auto _index = [&](size_t bytes, size_t align_shift) -> size_t
		{
			return ((bytes + (1 << align_shift) - 1) >> align_shift) - 1;
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
		else if (size <= static_cast<size_t>(8 * 1024))
		{
			return _index(size - 1024, 7) + 
				group_array[1] + 
				group_array[0];
		}
		else if (size <= static_cast<size_t>(64 * 1024))
		{
			return _index(size - static_cast<size_t>(8 * 1024), 10) + 
				group_array[2] + 
				group_array[1] + 
				group_array[0];
		}
		else if (size <= static_cast<size_t>(256 * 1024))
		{
			return _index(size - static_cast<size_t>(64 * 1024), 13) + 
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

	size_t MemoryPool::Bytes(size_t index)
	{
		static constexpr int group_array[4] = { 16, 56, 56, 56 };

		// align up to 8 (8 - 128 B)
		if (index < 16)
		{
			return 8 * (index + 1);
		}

		// align up to 16 (128B - 1024 B)
		else if(index < 16 + 56)
		{
			return 16 * (index + 1 - group_array[0]) + 128;
		}

		// align up to 128 (1 - 8 KB)
		else if (index < 16 + 56 + 56)
		{
			return 128 * (index + 1 - group_array[0] - group_array[1]) + 1024;
		}

		// align up to 1024 (8 - 64 KB)
		else if (index < 16 + 56 + 56 + 56)
		{
			return 1024 * (index + 1 - group_array[0] - group_array[1] - group_array[2]) + 8 * 1024;
		}

		// align up to 8KB (64 - 256 KB)
		else if (index < 16 + 56 + 56 + 56 + 24)
		{
			return 8 * 1024 * (index + 1 - group_array[0] - group_array[1] - group_array[2] - group_array[3]) + 64 * 1024;
		}

		else
		{
			SPICES_CORE_ERROR("Access invalid free_lits index")
			return 8 * 1024;
		}
	}

	size_t MemoryPool::GetNBlocksLimit(size_t size)
	{
		assert(size > 0);

		/**
		* @brief [2 - 512].
		*/
		size_t num = MAX_BYTES / size;
		num = std::max(std::min(static_cast<size_t>(512), num), static_cast<size_t>(2));

		return num;
	}

	size_t MemoryPool::GetPages(size_t size)
	{
		/**
		* @brief get blocks count.
		*/
		const size_t num = GetNBlocksLimit(size);

		/**
		* @brief get pages count.
		*/
		size_t npage = num * size;
		npage >>= PAGE_SHIFT;
		npage = std::max(npage, static_cast<size_t>(1));

		return npage;
	}
}