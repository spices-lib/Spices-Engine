/**
* @file NativeScriptSystem.cpp.
* @brief The NativeScriptSystem Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "NativeScriptSystem.h"
#include "Render/FrameInfo.h"
#include "World/Components/NativeScriptComponent.h"
#include "Slate/Imgui/ViewPort/ImguiViewport.h"
#include "World/World/World.h"
#include "Core/Event/Event.h"
#include "SlateSystem.h"

#include "Scripts/ViewPortResizeQueryer.h"
#include "Scripts/WorldPickIDQueryer.h"
#include "Scripts/WorldMarkQueryer.h"
#include "Scripts/NsightPrefReporterCaptureQueryer.h"

namespace Spices {

	void NativeScriptSystem::OnSystemInitialize()
	{
		m_NativeScriptRegister = std::make_unique<NativeScriptRegister>();

		m_NativeScriptRegister->Register<ViewPortResizeQueryer>();
		m_NativeScriptRegister->Register<WorldPickIDQueryer>();
		m_NativeScriptRegister->Register<WorldMarkQueryer>();
		m_NativeScriptRegister->Register<NsightPrefReporterCaptureQueryer>();
	}

	void NativeScriptSystem::OnSystemShutDown()
	{}

	void NativeScriptSystem::OnSystemUpdate(TimeStep& ts)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Update NativeScript( C++ );
		*/
		m_NativeScriptRegister->OnUpdate(ts);

		/**
		* @brief Update NativeScriptComponent( C++ in world );
		*/
		auto view = FrameInfo::Get().m_World->GetRegistry().view<NativeScriptComponent>();
		for (auto& e : view)
		{
			auto& nsComp = FrameInfo::Get().m_World->GetRegistry().get<NativeScriptComponent>(e);

			nsComp.OnTick(ts);
		}
	}

	void NativeScriptSystem::OnEvent(Event& event)
	{
		SPICES_PROFILE_ZONE;

		m_NativeScriptRegister->OnEvent(event);

		auto view = FrameInfo::Get().m_World->GetRegistry().view<NativeScriptComponent>();
		for (auto& e : view)
		{
			auto& nsComp = FrameInfo::Get().m_World->GetRegistry().get<NativeScriptComponent>(e);

			nsComp.OnEvent(event);
		}
	}
}