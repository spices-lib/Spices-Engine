#pragma once
#include "Core/Core.h"
#include "MemoryHelper.h"
#include "Core/Container/SpanList.h"

namespace Spices {

	class CenteralCache
	{
	public:

		static CenteralCache* Get() { return &m_CenteralCache; }

		CenteralCache() = default;
		virtual ~CenteralCache() = default;

		CenteralCache(const CenteralCache& copy) = delete;
		CenteralCache& operator =(const CenteralCache& copy) = delete;

		size_t FetchRangeObj(void*& start, void*& end, size_t batchNum, size_t size);
		span* GetOneSpan(span_list& list, size_t size);
		void* ReleaseListToSpans(void* start, size_t size);

	private:
		std::array<span_list, MemoryHelper::FREE_LIST_NUM> m_SpanLists;
		static CenteralCache m_CenteralCache;
	};
}