#include <EntryPoint.h>
#include "GameEditorWorld.h"

namespace Spiecs {

	std::shared_ptr<World> CreateWorld()
	{
		ZoneScoped;

		return std::make_shared<GameEditorWorld>();
	}
}