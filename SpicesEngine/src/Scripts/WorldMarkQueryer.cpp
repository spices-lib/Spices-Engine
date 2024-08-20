#include "Pchheader.h"
#include "WorldMarkQueryer.h"
#include "Render/FrameInfo.h"
#include "World/World/World.h"
#include "Core/Event/WorldEvent.h"
#include "Render/Vulkan/VulkanRenderBackend.h"

namespace Spices {

	WorldMarkQueryer::WorldMarkQueryer()
	{}

	void WorldMarkQueryer::OnTick(TimeStep& ts)
	{
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

		if (mark & World::ReBuildMaterial)
		{
			for (int i = 0; i < FrameInfo::Get().m_MaterialBuildEventQueue.size(); i++)
			{
				vkDeviceWaitIdle(VulkanRenderBackend::GetState().m_Device);

				FrameInfo::Get().m_MaterialBuildEventQueue[i]->BuildMaterial();
			}

			FrameInfo::Get().m_MaterialBuildEventQueue.clear();
			FrameInfo::Get().m_World->ClearMarkerWithBits(World::ReBuildMaterial);
		}
	}
}