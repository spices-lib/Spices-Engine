#pragma once
#include "Core/Core.h"

namespace Spices {

	void* ConcurrentAlloc(size_t size);

	void ConcurrentFree(void* obj);
}