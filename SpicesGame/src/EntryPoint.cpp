#include <EntryPoint.h>
#include "GameEditorWorld.h"
#include <Core/Memory/MemoryPool.h>

namespace Spices {

	std::shared_ptr<World> CreateWorld()
	{
		SPICES_PROFILE_ZONE;

		return std::make_shared<GameEditorWorld>();
	}
}