/**
* @file PageCache.cpp.
* @brief The PageCache Class Definitions.
* @author tcmalloc.
*/

#pragma once
#include "Core/Core.h"
#include "MemoryPool.h"
#include "Core/Container/SpanList.h"
#include "Core/Container/RadixTrie.h"
#include "ObjectPool.h"

namespace Spices {

	/**
	* @brief Page memory cache.
	* Third level of memory allocator.
	*/
	class PageCache
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		PageCache() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~PageCache() = default;

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaves.
		*/
		PageCache(const PageCache&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaves.
		*/
		PageCache& operator= (const PageCache&) = delete;

		/**
		* @brief Get this single instance.
		* @return Returns this single instance.
		*/
		static PageCache* Get() { return &m_PageCache; }

		/**
		* @brief Fetch pages span.
		* @param[in] k pages count.
		* @reurn Returns a new span.
		*/
		scl::span* NewSpan(size_t k);

		/**
		* @brief Find span by memory pointer.
		* @param[in] obj object memory pointer.
		* @return Returns span.
		*/
		scl::span* MapObjectToSpan(void* obj) const;

		/**
		* @brief Release span from cc to pc,
		* @param[in] s span.
		*/
		void ReleaseSpanToPageCache(scl::span* s);
		
	private:

		/**
		* @brief Fetch pages span(internal call).
		* @param[in] k pages count.
		* @reurn Returns a new span.
		*/
		scl::span* InternalNewSpan(size_t k);

	private:

		/**
		* @brief this single instance.
		*/
		static PageCache m_PageCache;

		/**
		* @brief FreeList Array.
		*/
		std::array<scl::span_list, MemoryPool::PAGE_NUM> m_SpanLists;

		/**
		* @brief ObjectPool for span.
		*/
		ObjectPool<scl::span> m_SpanPool;

		/**
		* @brief mutex for pc.
		*/
		std::mutex m_Mutex;

		/**
		* @brief radix trie for [pageId - span]
		*/
		scl::radix_trie<64 - MemoryPool::PAGE_SHIFT, 3> m_IdSpanMap;
	};
}