#include "Pchheader.h"
#include "ConcurrentAlloc.h"
#include "ThrealCache.h"

namespace Spices {

	void* ConcurrentAlloc(size_t size)
	{
		if (!pTLSThreadCache)
		{
			pTLSThreadCache = new ThreadCache;
		}

		return pTLSThreadCache->Allocate(size);
	}

	void ConcurrentFree(void* obj)
	{
		assert(obj);

		//pTLSThreadCache->Deallocate(obj, size);
	}
}