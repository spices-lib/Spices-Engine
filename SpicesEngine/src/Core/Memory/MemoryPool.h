#pragma once
#include "Core/Core.h"

namespace Spices {

	enum class MemorySizeUnit
	{
		B = 1,
		KB = 1024,
		MB = 1024 * 1024,
		GB = 1024 * 1024 * 1024,
	};

	struct FreeList
	{
		size_t    offset;
		size_t    size;
		FreeList* next;
	};

	class MemoryPool
	{
	public:

		bool CreatePool(size_t size, MemorySizeUnit unit);

	private:


		void* start;

		std::list<FreeList> m_FreeList;
	};
}