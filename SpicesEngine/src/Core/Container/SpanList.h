#pragma once

namespace Spices {

	class span
	{
	public:

		size_t m_PageId = 0;
		size_t n = 0;

		span* m_Next = nullptr;
		span* m_Prev = nullptr;

		void* m_FreeList = nullptr;
		size_t m_UseCount = 0;

		bool m_IsUse = false;
		size_t m_ObjSize = 0;
	};

	class span_list
	{
	public:

		span_list();

		span* Begin() { return m_Head->m_Next; }
		span* End() { return m_Head; }

		void PushFront(span* s);
		span* PopFront();
		bool Empty();
		void Insert(span* pos, span* ptr);
		void Erase(span* pos);
		std::mutex& GetMutex() { return m_Mutex; }

	private:
		span* m_Head;
		std::mutex m_Mutex;
	};

}