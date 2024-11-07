/**
* @file CentralCache.cpp.
* @brief The CentralCache Class Definitions.
* @author tcmalloc.
*/

#pragma once
#include "Core/Core.h"
#include "MemoryPool.h"
#include "Core/Container/SpanList.h"

namespace Spices {

	/**
	* @brief Central memory cache.
	* Second level of memory allocator.
	*/
	class CentralCache
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		CentralCache() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~CentralCache() = default;

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaves.
		*/
		CentralCache(const CentralCache&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaves.
		*/
		CentralCache& operator =(const CentralCache&) = delete;

		/**
		* @brief Get this single Instance.
		* @return Returns this pointer.
		*/
		static CentralCache* Get() { return &m_CentralCache; }

		/**
		* @brief Fetch range memory to tc.
		* @param[in,out] start memory start block pointer.
		* @param[in,out] end memory end block pointer.
		* @param[in] batchNum block count.
		* @param[in] size aligned bytes.
		* @return Returns actual block count.
		*/ 
		size_t FetchRange(void*& start, void*& end, size_t batchNum, size_t size);

		/**
		* @brief Release memory to pc.
		* @param[in] start memory start pointer.
		* @param[in] size aligned bytes.
		*/
		void ReleaseListToSpans(void* start, size_t size);

	private:

		/**
		* @brief Get a not empty span.
		* @param[in,out] list span list.
		* @param[in] size aligned bytes.
		* @return Returns span.
		*/
		static scl::span* GetOneSpan(scl::span_list& list, size_t size);

	private:

		/**
		* @brief FreeList Array.
		*/
		std::array<scl::span_list, MemoryPool::FREE_LIST_NUM> m_SpanLists;

		/**
		* @brief Single instance of this.
		*/
		static CentralCache m_CentralCache;
	};
}