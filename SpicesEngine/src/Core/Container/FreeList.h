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

	private:
		void* m_Freelist;
	};
}