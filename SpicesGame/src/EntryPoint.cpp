#include <EntryPoint.h>
#include "GameEditorWorld.h"

#include <new>

#if TRACY_ENABLE && 0

void* operator new(size_t size)
{
    void* ptr = malloc(size);
	SPICES_PROFILE_ALLOC(ptr, size);
    return ptr;
}

void operator delete(void* ptr) noexcept
{
	SPICES_PROFILE_FREE(ptr);
	free(ptr);
}

#endif

namespace Spices {

	std::shared_ptr<World> CreateWorld()
	{
		ZoneScoped;

		return std::make_shared<GameEditorWorld>();
	}
}