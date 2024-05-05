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
		/**
		* @brief Temp, query Slate resize event.
		*/
		m_ViewPortResizeQueryer->QueryEvent(ts);

		auto& view = FrameInfo::Get().m_World->GetRegistry().view<NativeScriptComponent>();
		for (auto& e : view)
		{
			auto& nsComp = FrameInfo::Get().m_World->GetRegistry().get<NativeScriptComponent>(e);

			nsComp.OnTick(ts);
		}
	}

	void NativeScriptSystem::OnEvent(Event& event)
	{
		/**
		* @breif Only active world's NativeScriptComponent event while viewport is hovered.
		*/
		if (!SlateSystem::GetRegister()->GetViewPort()->IsHovered()) return;

		auto& view = FrameInfo::Get().m_World->GetRegistry().view<NativeScriptComponent>();
		for (auto& e : view)
		{
			auto& nsComp = FrameInfo::Get().m_World->GetRegistry().get<NativeScriptComponent>(e);

			nsComp.OnEvent(event);
		}
	}
}