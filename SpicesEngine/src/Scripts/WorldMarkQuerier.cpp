/**
* @file WorldMarkQuerier.cpp.
* @brief The WorldMarkQuerier & NativeScriptRegister Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "WorldMarkQuerier.h"
#include "Render/FrameInfo.h"
#include "World/World/World.h"
#include "Core/Event/WorldEvent.h"
#include "Render/Vulkan/VulkanRenderBackend.h"

namespace Spices {

	void WorldMarkQuerier::OnTick(TimeStep& ts)
	{
		SPICES_PROFILE_ZONE;
		
		World::WorldMarkFlags mark = FrameInfo::Get().m_World->GetMarker();

		if (mark & World::MeshAddedToWorld)
		{
			MeshAddedWorldEvent event;
			Event::GetEventCallbackFn()(event);

			FrameInfo::Get().m_World->ClearMarkerWithBits(World::MeshAddedToWorld);
		}

		if (mark & World::FrushStableFrame)
		{
			auto view = FrameInfo::Get().m_World->GetRegistry().view<CameraComponent>();
			for (auto& e : view)
			{
				auto& camComp = FrameInfo::Get().m_World->GetRegistry().get<CameraComponent>(e);

				if (camComp.IsActive())
				{
					camComp.GetCamera()->ResetStableFrames();
				}
			}

			FrameInfo::Get().m_World->ClearMarkerWithBits(World::FrushStableFrame);
		}
	}
}