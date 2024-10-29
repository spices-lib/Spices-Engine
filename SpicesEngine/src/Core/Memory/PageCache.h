#pragma once
#include "Core/Core.h"
#include "MemoryHelper.h"
#include "Core/Container/SpanList.h"

namespace Spices {

	class PageCache
	{
	public:
		PageCache() = default;
		virtual ~PageCache() = default;

		PageCache(const PageCache& copy) = delete;
		PageCache& operator= (const PageCache& copy) = delete;

		static PageCache* Get() { return &m_PageCache; }
		span* NewSpan(size_t k);

		std::mutex& GetMutex() { return m_Mutex; }

		span* MapObjectToSpan(void* obj);
		void ReleaseSpanToPageCache(span* s);

	private:
		static PageCache m_PageCache;
		std::array<span_list, MemoryHelper::PAGE_NUM> m_SpanLists;
		std::mutex m_Mutex;
		std::unordered_map<size_t, span*> m_IdSpanMap;
	};
}