#pragma once

#include "tracy/Tracy.hpp"
#include "tracy/TracyC.h"

#ifdef TRACY_ENABLE

// Profile macros
#define SPICES_PROFILE_FRAME             FrameMark
#define SPICES_PROFILE_ZONE              ZoneScoped
#define SPICES_PROFILE_ZONEN(name)       ZoneScopedN(name)
#define SPICES_PROFILE_ALLOC(ptr, size)  TracyAllocS(ptr, size, 20)
#define SPICES_PROFILE_FREE(ptr)         TracyFreeS(ptr, 20)

#else

// Profile macros
#define SPICES_PROFILE_FRAME
#define SPICES_PROFILE_ZONE
#define SPICES_PROFILE_ZONEN(name)
#define SPICES_PROFILE_ALLOC(ptr, size)
#define SPICES_PROFILE_FREE(ptr)
   
#endif // SPICES_RELEASE