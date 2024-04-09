#include "pchheader.h"
#include "NativeScriptSystem.h"
#include "Render/FrameInfo.h"
#include "World/Components/NativeScriptComponent.h"
#include "World/World/World.h"

namespace Spiecs {

	void NativeScriptSystem::OnSystemInitialize()
	{

	}

	void NativeScriptSystem::OnSystemShutDown()
	{

	}

	void NativeScriptSystem::OnSystemUpdate(TimeStep ts)
	{
		auto& view = FrameInfo::Get().m_World->GetRegistry().view<NativeScriptComponent>();
		for (auto& e : view)
		{
			auto& nsComp = FrameInfo::Get().m_World->GetRegistry().get<NativeScriptComponent>(e);

			nsComp.OnTick();
		}
	}
}