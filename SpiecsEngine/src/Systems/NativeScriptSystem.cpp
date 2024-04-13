#include "Pchheader.h"
#include "NativeScriptSystem.h"
#include "Render/FrameInfo.h"
#include "World/Components/NativeScriptComponent.h"
#include "World/World/World.h"
#include "Core/Event/Event.h"

namespace Spiecs {

	void NativeScriptSystem::OnSystemInitialize()
	{

	}

	void NativeScriptSystem::OnSystemShutDown()
	{

	}

	void NativeScriptSystem::OnSystemUpdate(TimeStep& ts)
	{
		auto& view = FrameInfo::Get().m_World->GetRegistry().view<NativeScriptComponent>();
		for (auto& e : view)
		{
			auto& nsComp = FrameInfo::Get().m_World->GetRegistry().get<NativeScriptComponent>(e);

			nsComp.OnTick(ts);
		}
	}

	void NativeScriptSystem::OnEvent(Event& event)
	{
		auto& view = FrameInfo::Get().m_World->GetRegistry().view<NativeScriptComponent>();
		for (auto& e : view)
		{
			auto& nsComp = FrameInfo::Get().m_World->GetRegistry().get<NativeScriptComponent>(e);

			nsComp.OnEvent(event);
		}
	}
}