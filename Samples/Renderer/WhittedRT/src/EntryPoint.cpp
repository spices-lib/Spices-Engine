#include <EntryPoint.h>
#include "WhittedRTWorld.h"

namespace Spices {

	std::shared_ptr<World> CreateWorld()
	{
		SPICES_PROFILE_ZONE;

		return std::make_shared<WhittedRTWorld>();
	}
}