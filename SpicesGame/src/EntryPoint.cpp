#include <EntryPoint.h>
#include "GameEditorWorld.h"
#include <Core/Memory/MemoryPool.h>

#include <new>

#if 0

void* operator new(size_t size)
{
    void* ptr = Spices::MemoryPool::Alloc(size);
    return ptr;
}

void operator delete(void* ptr) noexcept
{
	Spices::MemoryPool::Free(ptr);
}

#endif

namespace Spices {

	std::shared_ptr<World> CreateWorld()
	{
		SPICES_PROFILE_ZONE;

		return std::make_shared<GameEditorWorld>();
	}
}