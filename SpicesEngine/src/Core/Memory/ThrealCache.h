/**
* @file ThrealCache.cpp.
* @brief The ThrealCache Class Definitions.
* @author tcmalloc.
*/

#pragma once
#include "Core/Core.h"
#include "Memory.h"
#include "Core/Container/FreeList.h"

namespace Spices {

	class ThreadCache
	{
	public:

		void* Allocate(size_t size);
		void Deallocate(void* obj, size_t size);

	private:
		FreeList m_FreeList[];
	};
}