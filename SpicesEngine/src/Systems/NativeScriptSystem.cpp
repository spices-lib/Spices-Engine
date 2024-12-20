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

#include "Scripts/ViewPortResizeQuerier.h"
#include "Scripts/WorldPickIDQuerier.h"
#include "Scripts/WorldMarkQuerier.h"
#include "Scripts/MainTaskQuerier.h"

namespace Spices {

	void NativeScriptSystem::OnSystemInitialize()
	{
		m_NativeScriptRegister = std::make_unique<NativeScriptRegister>();

		m_NativeScriptRegister->Register<MainTaskQuerier>();
		m_NativeScriptRegister->Register<ViewPortResizeQuerier>();
		m_NativeScriptRegister->Register<WorldPickIDQuerier>();
		m_NativeScriptRegister->Register<WorldMarkQuerier>();
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
		FrameInfo::Get().m_World->ViewComponent<NativeScriptComponent>([&](auto e, auto& nsComp){
			nsComp.OnTick(ts);
			return false;
		});
	}

	void NativeScriptSystem::OnEvent(Event& event)
	{
		SPICES_PROFILE_ZONE;

		m_NativeScriptRegister->OnEvent(event);

		FrameInfo::Get().m_World->ViewComponent<NativeScriptComponent>([&](auto e, auto& nsComp){
			nsComp.OnEvent(event);
			return false;
		});
	}
}