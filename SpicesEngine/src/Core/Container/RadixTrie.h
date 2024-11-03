/**
* @file RadixTrie.h.
* @brief The RadixTrie Class Definitions and Implementation.
* @author Spices.
*/

#pragma once
#include "Core/Memory/MemoryPool.h"

namespace Spices {

	template<typename size_t BITS>
	class radix_trie_1
	{
	private:
		static constexpr size_t LENGTH = 1 << BITS;
		void** array_;
	public:

		explicit radix_trie_1()
		{
			size_t size = sizeof(void*) << BITS;
			size_t alignSize = MemoryPool::AlignUp(size, 1 << MemoryPool::PAGE_SHIFT);
			array_ = (void**)SystemAlloc(alignSize >> MemoryPool::PAGE_SHIFT);
			memset(array_, 0, sizeof(void*) << BITS);
		}

		void* get(size_t k) const
		{
			if ((k >> BITS) > 0)
			{
				return nullptr;
			}
			return array_[k];
		}

		void set(size_t k, void* v)
		{
			array_[k] = v;
		}
	};
}