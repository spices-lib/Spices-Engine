#pragma once

#include "tracy/Tracy.hpp"
#include "tracy/TracyC.h"

#ifdef SPIECS_DEBUG

// Profile macros
#define SPIECS_PROFILE_FRAME             FrameMark
#define SPIECS_PROFILE_ZONE              ZoneScoped
#define SPIECS_PROFILE_ZONEN(name)       ZoneScopedN(name)
#define SPIECS_PROFILE_ALLOC(ptr, size)  TracyAllocS(ptr, size, 20)
#define SPIECS_PROFILE_FREE(ptr)         TracyFreeS(ptr, 20)

#endif // SPIECS_DEBUG

#ifdef SPIECS_RELEASE

// Profile macros
#define SPIECS_PROFILE_FRAME
#define SPIECS_PROFILE_ZONE
#define SPIECS_PROFILE_ZONEN(name)
#define SPIECS_PROFILE_ALLOC(ptr, size)
#define SPIECS_PROFILE_FREE(ptr)
   
#endif // SPIECS_RELEASE