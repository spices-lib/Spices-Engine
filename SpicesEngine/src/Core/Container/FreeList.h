/**
* @file FreeList.h.
* @brief The FreeList Class Definitions.
* @author Spices.
*/

#pragma once

namespace Spices {

	class FreeList
	{
	public:
		void Push(void* obj);
		void* Pop();
		bool Empty() { return m_Freelist == nullptr; }
	private:
		void* m_Freelist;
	};
}