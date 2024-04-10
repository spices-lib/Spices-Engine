#include "EntryPoint.h"
#include "GameEditorWorld.h"

namespace Spiecs {

	std::shared_ptr<World> CreateWorld()
	{
		return std::make_shared<GameEditorWorld>();
	}
}