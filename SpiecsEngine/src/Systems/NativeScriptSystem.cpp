/**
* @file NativeScriptSystem.cpp.
* @brief The NativeScriptSystem Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "NativeScriptSystem.h"
#include "Render/FrameInfo.h"
#include "World/Components/NativeScriptComponent.h"
#include "Slate/Imgui/ViewPort/ImguiViewport.h"
#include "World/World/World.h"
#include "Core/Event/Event.h"
#include "SlateSystem.h"

namespace Spiecs {

	void NativeScriptSystem::OnSystemInitialize()
	{
		m_ViewPortResizeQueryer = std::make_unique<ViewPortResizeQueryer>();
	}

	void NativeScriptSystem::OnSystemShutDown()
	{}

	void NativeScriptSystem::OnSystemUpdate(TimeStep& ts)
	{
		m_ViewPortResizeQueryer->QueryEvent();

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