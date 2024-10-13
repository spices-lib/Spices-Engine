#include <EntryPoint.h>
#include "WhittedRTWorld.h"

#include <new>

#ifdef TRACY_ENABLE
#if 0

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
#endif

namespace Spices {

	std::shared_ptr<World> CreateWorld()
	{
		SPICES_PROFILE_ZONE;;

		return std::make_shared<WhittedRTWorld>();
	}
}