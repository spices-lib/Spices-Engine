#include <EntryPoint.h>
#include "GameEditorWorld.h"

#include <new>

#if TRACY_ENABLE && 1

void* operator new(size_t size)
{
    void* ptr = malloc(size);
	SPIECS_PROFILE_ALLOC(ptr, size);
    return ptr;
}

void operator delete(void* ptr) noexcept
{
	SPIECS_PROFILE_FREE(ptr);
	free(ptr);
}

#endif

namespace Spiecs {

	std::shared_ptr<World> CreateWorld()
	{
		ZoneScoped;

		return std::make_shared<GameEditorWorld>();
	}
}