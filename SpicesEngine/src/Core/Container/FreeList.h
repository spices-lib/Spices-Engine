/**
* @file FreeList.h.
* @brief The free_list Class Definitions.
* @author Spices.
*/

#pragma once

namespace Spices {

	class free_list
	{
	public:
		void Push(void* obj);
		void* Pop();
		bool Empty() { return m_Freelist == nullptr; }
		size_t& MaxSize() { return m_MaxSize; }
		void PushRange(void* start, void* end, size_t size);
		void PopRange(void*& start, void*& end, size_t n);
		size_t Size() { return m_Size; }
	private:
		void* m_Freelist;
		size_t m_MaxSize = 1;
		size_t m_Size = 0;
	};
}