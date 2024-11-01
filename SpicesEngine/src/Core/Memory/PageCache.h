#pragma once
#include "Core/Core.h"
#include "MemoryHelper.h"
#include "Core/Container/SpanList.h"
#include "ObjectPool.h"

namespace Spices {

	class PageCache
	{
	public:
		PageCache() = default;
		virtual ~PageCache() = default;

		PageCache(const PageCache& copy) = delete;
		PageCache& operator= (const PageCache& copy) = delete;

		static PageCache* Get() { return &m_PageCache; }
		scl::span* NewSpan(size_t k);

		std::mutex& GetMutex() { return m_Mutex; }

		scl::span* MapObjectToSpan(void* obj);
		void ReleaseSpanToPageCache(scl::span* s);

	private:
		static PageCache m_PageCache;
		std::array<scl::span_list, MemoryHelper::PAGE_NUM> m_SpanLists;
		ObjectPool<scl::span> m_SpanPool;
		std::mutex m_Mutex;
		std::unordered_map<size_t, scl::span*> m_IdSpanMap;
	};
}