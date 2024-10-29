/**
* @file FreeList.h.
* @brief The FreeList Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "FreeList.h"
#include "Core/Memory/MemoryHelper.h"

namespace Spices {

	void FreeList::Push(void* obj)
	{
		assert(obj);

		MemoryHelper::ObjNext(obj) = m_Freelist;
		m_Freelist   = obj;
	}

	void* FreeList::Pop()
	{
		assert(m_Freelist);

		void* obj  = m_Freelist;
		m_Freelist = MemoryHelper::ObjNext(obj);

		return obj;
	}
}