/**
* @file CenteralCache.cpp.
* @brief The CenteralCache Class Definitions.
* @author tcmalloc.
*/

#pragma once
#include "Core/Core.h"
#include "MemoryHelper.h"
#include "Core/Container/SpanList.h"

namespace Spices {

	/**
	* @brief Centeral memory cache.
	* Second level of memory allocator.
	*/
	class CenteralCache
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		CenteralCache() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~CenteralCache() = default;

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaves.
		*/
		CenteralCache(const CenteralCache&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaves.
		*/
		CenteralCache& operator =(const CenteralCache&) = delete;

		/**
		* @brief Get this single Instance.
		* @return Returns this pointer.
		*/
		static CenteralCache* Get() { return &m_CenteralCache; }

		/**
		* @brief Fetch range memory to tc.
		* @param[in,out] start memory start block pointer.
		* @param[in,out] end memory end block pointer.
		* @param[in] batchNum block count.
		* @param[in] size aligned bytes.
		* @return Returns actural block count.
		*/ 
		size_t FetchRange(void*& start, void*& end, size_t batchNum, size_t size);

		/**
		* @brief Get a not empty span.
		* @param[in,out] list span list.
		* @param[in] size aligned bytes.
		* @return Returns span.
		*/
		scl::span* GetOneSpan(scl::span_list& list, size_t size);
		void ReleaseListToSpans(void* start, size_t size);

	private:

		/**
		* @brief FreeList Array.
		*/
		std::array<scl::span_list, MemoryHelper::FREE_LIST_NUM> m_SpanLists;

		/**
		* @brief Single instance of this.
		*/
		static CenteralCache m_CenteralCache;
	};
}