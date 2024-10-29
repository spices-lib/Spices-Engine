#include "Pchheader.h"
#include "MemoryHelper.h"
#include "Core/Library/MemoryLibrary.h"

namespace Spices {

	void*& MemoryHelper::ObjNext(void* obj)
	{
		return *(void**)obj;
	}

	size_t MemoryHelper::AlignUp(size_t size)
	{
		if (size <= 128)
		{
			return MemoryLibrary::align_up<size_t>(size, 8);
		}
		else if (size <= 1024)
		{
			return MemoryLibrary::align_up<size_t>(size, 16);
		}
		else if (size <= 8 * 1024)
		{
			return MemoryLibrary::align_up<size_t>(size, 128);
		}
		else if (size <= 64 * 1024)
		{
			return MemoryLibrary::align_up<size_t>(size, 1024);
		}
		else if (size <= 256 * 1024)
		{
			return MemoryLibrary::align_up<size_t>(size, 8 * 1024);
		}
		else
		{
			assert(false);
			return -1;
		}
	}

	size_t MemoryHelper::Index(size_t size)
	{
		assert(size <= MAX_BYTES);

		auto _index = [&](size_t s, size_t align_shift) {
			return ((s + (1 << align_shift) - 1) >> align_shift) - 1;
		};

		static int group_array[4] = { 16, 56, 56, 56 };
		if (size <= 128)
		{
			return _index(size, 3);
		}
		else if (size <= 1024)
		{
			return _index(size - 128, 4) + group_array[0];
		}
		else if (size <= 8 * 1024)
		{
			return _index(size - 1024, 7) + group_array[1] + group_array[0];
		}
		else if (size <= 64 * 1024)
		{
			return _index(size - 8 * 1024, 10) + group_array[2] + group_array[1] + group_array[0];
		}
		else if (size <= 256 * 1024)
		{
			return _index(size - 64 * 1024, 13) + group_array[3] + 
				group_array[2] + group_array[1] + group_array[0];
		}
		else
		{
			assert(false);
			return -1;
		}
	}

	size_t MemoryHelper::NumMoveSize(size_t size)
	{
		assert(size > 0);

		int num = MAX_BYTES / size;

		if (num > 512)
		{
			num = 512;
		}

		if (num < 2)
		{
			num = 2;
		}

		return num;
	}

	size_t MemoryHelper::NumMovePage(size_t size)
	{
		size_t num = NumMoveSize(size);

		size_t npage = num * size;

		npage >>= PAGE_SHIFT;

		if (npage == 0)
		{
			npage = 1;
		}

		return npage;
	}

}