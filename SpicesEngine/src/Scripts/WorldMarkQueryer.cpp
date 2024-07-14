#include "Pchheader.h"
#include "WorldMarkQueryer.h"
#include "Render/FrameInfo.h"
#include "World/World/World.h"
#include "Core/Event/WorldEvent.h"

namespace Spices {

	WorldMarkQueryer::WorldMarkQueryer()
	{}

	void WorldMarkQueryer::OnTick(TimeStep& ts)
	{
		WorldMarkFlags mark = FrameInfo::Get().m_World->GetMarker();

		if (mark & WorldMarkBits::MeshAddedToWorld)
		{
			MeshAddedWorldEvent event;
			Event::GetEventCallbackFn()(event);
		}

		FrameInfo::Get().m_World->ReserMark();
	}
}